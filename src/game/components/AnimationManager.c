#include "raylib.h"
#include "rlgl.h"
#include "../g.h"
#include "shared/scene_graph/scene_graph.h"
#include "shared/resource_manager.h"
#include <external/cjson.h>

AnimationManager *AnimationManager_getInstance(SceneGraph *sceneGraph)
{
    AnimationManager *animationManager;
    SceneGraph_getComponentByType(sceneGraph, (SceneObjectId){0}, _componentIdMap.AnimationManagerId, (void **)&animationManager, 0);
    if (animationManager == NULL)
    {
        SceneObjectId id = SceneGraph_createObject(sceneGraph, "AnimationManager");
        SceneComponentId componentId = SceneGraph_addComponent(sceneGraph, id, _componentIdMap.AnimationManagerId, &(AnimationManager){0});
        if (SceneGraph_getComponent(sceneGraph, componentId, (void **)&animationManager) == NULL)
        {
            TraceLog(LOG_ERROR, "Failed to get AnimationManager component");
            return NULL;
        }
    }
    return animationManager;
}

void AnimationTrack_cleanup(AnimationTrack *track)
{
    if (track->path)
    {
        RL_FREE(track->path);
        if (track->keys != NULL)
        {
            RL_FREE(track->keys);
        }
        track->keys_capacity = track->keys_count = 0;
    }
}

void AnimationClip_cleanup(AnimationClip *clip)
{
    if (clip->name)
    {
        RL_FREE(clip->name);
    }

    if (clip->tracks != NULL)
    {
        for (int i = 0; i < clip->tracks_count; i++)
        {
            AnimationTrack_cleanup(&clip->tracks[i]);
        }
        clip->tracks_capacity = clip->tracks_count = 0;
        if (clip->tracks)
            RL_FREE(clip->tracks);
        clip->tracks = NULL;
    }
}

void Animation_cleanup(Animation *animation)
{
    if (animation->clips_count > 0)
    {
        for (int i = 0; i < animation->clips_count; i++)
        {
            AnimationClip_cleanup(&animation->clips[i]);
        }
        animation->clips_capacity = animation->clips_count = 0;
        if (animation->clips)
            RL_FREE(animation->clips);
        animation->clips = NULL;
    }
}

void Animation_load(Animation *animation)
{
    printf("Loading Spritesheet Animation: %s\n", animation->filename);

    char *content = ResourceManager_loadText(_resourceManager, animation->filename);
    if (content == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load Spritesheet Animation: %s", animation->filename);
        return;
    }

    cJSON *root = cJSON_Parse(content);
    if (root == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__);
        goto error;
    }

#define GET_ARRAY(obj, name)                                                                     \
    cJSON *name = cJSON_GetObjectItem(obj, #name);                                               \
    if (!cJSON_IsArray(name))                                                                    \
    {                                                                                            \
        TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__); \
        goto error;                                                                            \
    }

#define GET_OBJECT(obj, name)                                                                    \
    cJSON *name = cJSON_GetObjectItem(obj, #name);                                               \
    if (!cJSON_IsObject(name))                                                                   \
    {                                                                                            \
        TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__); \
        goto error;                                                                            \
    }

#define GET_STRING(obj, name)                                                                    \
    cJSON *strElement##name = cJSON_GetObjectItem(obj, #name);                                   \
    if (!cJSON_IsString(strElement##name))                                                       \
    {                                                                                            \
        TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__); \
        goto error;                                                                            \
    }                                                                                            \
    const char *name = strElement##name->valuestring;

#define GET_FLOAT(obj, name)                                                                     \
    cJSON *floatElement##name = cJSON_GetObjectItem(obj, #name);                                 \
    if (!cJSON_IsNumber(floatElement##name))                                                     \
    {                                                                                            \
        TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__); \
        goto error;                                                                            \
    }                                                                                            \
    float name = (float)floatElement##name->valuedouble;

    GET_ARRAY(root, clips);
    GET_ARRAY(root, states);
    int clipCount = cJSON_GetArraySize(clips);
    if (clipCount > animation->clips_capacity)
    {
        animation->clips_capacity = clipCount;
        animation->clips = (AnimationClip *)RL_REALLOC(animation->clips, sizeof(AnimationClip) * animation->clips_capacity);
    }

    for (int i = 0; i < clipCount; i++)
    {
        cJSON *clip = cJSON_GetArrayItem(clips, i);
        GET_OBJECT(clip, tracks);
        GET_STRING(clip, name);
        GET_FLOAT(clip, duration);
        int trackCount = cJSON_GetArraySize(tracks);
        AnimationClip animationClip = {
            .name = RL_STRDUP(name),
            .duration = duration,
            .tracks = RL_MALLOC(sizeof(AnimationTrack) * trackCount),
            .tracks_count = trackCount,
        };
        animation->clips[i] = animationClip;

        for (int j = 0; j < trackCount; j++)
        {
            cJSON *track = cJSON_GetArrayItem(tracks, j);
            if (!cJSON_IsObject(track))
            {
                TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__);
                goto error;
            }
            const char *trackPath = track->string;
            GET_ARRAY(track, time);
            GET_ARRAY(track, value);
            int keyCount = cJSON_GetArraySize(time);
            int valueCount = cJSON_GetArraySize(value);
            int count = keyCount < valueCount ? keyCount : valueCount;
            AnimationTrack animationTrack = {
                .path = RL_STRDUP(trackPath),
                .keys = RL_MALLOC(sizeof(AnimationKey) * count),
                .keys_count = count,
                .keys_capacity = count,
            };
            animationClip.tracks[j] = animationTrack;
            for (int k = 0; k < count; k++)
            {
                cJSON *timeElement = cJSON_GetArrayItem(time, k);
                cJSON *valueElement = cJSON_GetArrayItem(value, k);
                if (!cJSON_IsNumber(timeElement) || !cJSON_IsNumber(valueElement))
                {
                    TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__);
                    goto error;
                }
                animationTrack.keys[k] = (AnimationKey){
                    .time = (float)timeElement->valuedouble,
                    .value = (float)valueElement->valuedouble,
                };
            }
        }
    }

    animation->clips_count = clipCount;

    printf("Loaded Spritesheet Animation: %s\n", animation->filename);

    goto cleanup;
#undef GET_ARRAY
#undef GET_STRING
#undef GET_FLOAT

error:
    Animation_cleanup(animation);
cleanup:
    cJSON_Delete(root);
}

STRUCT_LIST_ACQUIRE_FN(AnimationManager, Animation, animations)

AnimationId AnimationManager_getAnimation(AnimationManager *manager, char *filename)
{
    for (int i = 0; i < manager->animations_count; i++)
    {
        if (strcmp(manager->animations[i].filename, filename) == 0)
        {
            return (AnimationId){i, manager->animations[i].generation};
        }
    }
    Animation *animation = AnimationManager_acquire_animations(manager);
    animation->clips_count = 0;
    animation->clips = NULL;
    animation->clips_capacity = 0;
    animation->lastModifiedTime = ResourceManager_getModHash(_resourceManager, filename);
    animation->filename = filename;
    animation->generation = ++manager->generationCounter;
    Animation_load(animation);
    return (AnimationId){manager->animations_count - 1, animation->generation};
}

Animation* AnimationManager_getAnimationById(AnimationManager *manager, AnimationId id)
{
    if (id.generation == 0)
    {
        return NULL;
    }
    if (id.animationIndex >= manager->animations_count)
    {
        return NULL;
    }
    Animation *animation = &manager->animations[id.animationIndex];
    if (animation->generation != id.generation)
    {
        return NULL;
    }
    return animation;
}

static void AnimationManager_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                    float delta, void *componentData)
{
    AnimationManager *component = (AnimationManager *)componentData;
    for (int i = 0; i < component->animations_count; i++)
    {
        Animation *animation = &component->animations[i];
        
        int modTime = ResourceManager_getModHash(_resourceManager, animation->filename);
        if (modTime != animation->lastModifiedTime)
        {
            animation->lastModifiedTime = modTime;
            Animation_load(animation);
        }
    }
}

BEGIN_COMPONENT_REGISTRATION(AnimationManager){
    .updateTick = AnimationManager_update,
} END_COMPONENT_REGISTRATION