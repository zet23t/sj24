#include "game/g.h"

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

void AnimationState_cleanup(AnimationState *state)
{
    if (state->name)
    {
        RL_FREE(state->name);
    }

    if (state->clipSequence != NULL)
    {
        if (state->clipSequence)
            RL_FREE(state->clipSequence);
        state->clipSequence = NULL;
        state->clipSequence_capacity = state->clipSequence_count = 0;
    }

    if (state->transitions != NULL)
    {
        for (int i = 0; i < state->transitions_count; i++)
        {
            AnimationStateTransition *transition = &state->transitions[i];
            for (int j = 0; j < transition->conditions_count; j++)
            {
                AnimationCondition *condition = &transition->conditions[j];
                if (condition->varA)
                {
                    RL_FREE(condition->varA);
                }
                if (condition->varB)
                {
                    RL_FREE(condition->varB);
                }
                if (condition->operation)
                {
                    RL_FREE(condition->operation);
                }
            }
        }
        state->transitions_capacity = state->transitions_count = 0;
        if (state->transitions)
            RL_FREE(state->transitions);
        state->transitions = NULL;
    }
}

void AnimationVariable_cleanup(AnimationVariable *variable)
{
    if (variable->name)
    {
        RL_FREE(variable->name);
    }
}

void AnimationCondition_cleanup(AnimationCondition *condition)
{
    if (condition->varA)
    {
        RL_FREE(condition->varA);
    }
    if (condition->varB)
    {
        RL_FREE(condition->varB);
    }
    if (condition->operation)
    {
        RL_FREE(condition->operation);
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

    if (animation->states_count > 0)
    {
        for (int i = 0; i < animation->states_count; i++)
        {
            AnimationState_cleanup(&animation->states[i]);
        }
        animation->states_capacity = animation->states_count = 0;
        if (animation->states)
            RL_FREE(animation->states);
        animation->states = NULL;
    }

    if (animation->variables_count > 0)
    {
        for (int i = 0; i < animation->variables_count; i++)
        {
            AnimationVariable_cleanup(&animation->variables[i]);
        }
        animation->variables_capacity = animation->variables_count = 0;
        if (animation->variables)
            RL_FREE(animation->variables);
        animation->variables = NULL;
    }
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

static int Animation_loadVariables(Animation *animation, cJSON *variables)
{
    int variableCount = cJSON_GetArraySize(variables);
    if (variableCount > animation->variables_capacity)
    {
        animation->variables = (AnimationVariable *)RL_REALLOC(animation->variables, sizeof(AnimationVariable) * variableCount);
        for (int j=animation->variables_count;j<variableCount;j++)
        {
            animation->variables[j] = (AnimationVariable){
                .name = NULL,
                .value = 0,
            };
        }
        animation->variables_capacity = variableCount;
    }

    animation->variables_count = 0;

    for (int i = 0; i < variableCount; i++)
    {
        cJSON *variable = cJSON_GetArrayItem(variables, i);
        GET_FLOAT(variable, value);
        animation->variables[i] = (AnimationVariable){
            .name = RL_STRDUP(variable->string),
            .value = value,
        };
        animation->variables_count++;
    }

    return 0;

    error: 
    return 1;
}

static int Animation_loadClips(Animation* animation, cJSON* clips)
{
    
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

    return 0;
    error:
    return 1;
}

static int Animation_loadStates(Animation *animation, cJSON *states)
{
    int stateCount = cJSON_GetArraySize(states);
    if (stateCount > animation->states_capacity)
    {
        animation->states_capacity = stateCount;
        animation->states = (AnimationState *)RL_REALLOC(animation->states, sizeof(AnimationState) * animation->states_capacity);
    }

    for (int i = 0; i < stateCount; i++)
    {
        cJSON *state = cJSON_GetArrayItem(states, i);
        GET_STRING(state, name);
        GET_ARRAY(state, sequence);
        GET_ARRAY(state, transitions);
        int clipSequenceCount = cJSON_GetArraySize(sequence);
        int transitionsCount = cJSON_GetArraySize(transitions);
        AnimationState animationState = {
            .name = RL_STRDUP(name),
            .clipSequence = RL_MALLOC(sizeof(int) * clipSequenceCount),
            .clipSequence_capacity = clipSequenceCount,
            .transitions = RL_MALLOC(sizeof(AnimationCondition) * transitionsCount),
            .transitions_capacity = transitionsCount,
        };
        animation->states[i] = animationState;

        // animation->states[i].transitions_count = transitionsCount;

        for (int j = 0; j < clipSequenceCount; j++)
        {
            cJSON *clipElement = cJSON_GetArrayItem(sequence, j);
            if (!cJSON_IsString(clipElement))
            {
                TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__);
                goto error;
            }
            const char *clipName = clipElement->valuestring;
            int clipIndex = -1;
            for (int k = 0; k < animation->clips_count; k++)
            {
                if (strcmp(animation->clips[k].name, clipName) == 0)
                {
                    clipIndex = k;
                    break;
                }
            }
            if (clipIndex == -1)
            {
                TraceLog(LOG_ERROR, "Unknown clipname %s to load Animation: %s (%d)", clipName, animation->filename, __LINE__);
                goto error;
            }
            animation->states[i].clipSequence[j] = clipIndex;
            animation->states[i].clipSequence_count = j + 1;
        }

        for (int j = 0; j < transitionsCount; j++)
        {
            cJSON *transition = cJSON_GetArrayItem(transitions, j);
            GET_STRING(transition, target);
            GET_ARRAY(transition, conditions);
            int conditionsCount = cJSON_GetArraySize(conditions);
            AnimationStateTransition animationTransition = {
                .target = RL_STRDUP(target),
                .conditions = RL_MALLOC(sizeof(AnimationCondition) * conditionsCount),
                .conditions_capacity = conditionsCount,
            };
            for (int k = 0; k < conditionsCount; k++)
            {
                animationTransition.conditions[k] = (AnimationCondition){
                    .varA = NULL,
                    .varB = NULL,
                };
            }
            animation->states[i].transitions[j] = animationTransition;

            for (int k = 0; k < conditionsCount; k++)
            {
                
            }
        }
    }

    animation->states_count = stateCount;

    return 0;
    error:
    return 1;
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

    GET_ARRAY(root, clips);
    GET_ARRAY(root, states);
    GET_OBJECT(root, variables);

    if (Animation_loadVariables(animation, variables) != 0)
    {
        goto error;
    }

    if (Animation_loadClips(animation, clips) != 0)
    {
        goto error;
    }

    if (Animation_loadStates(animation, states) != 0)
    {
        goto error;
    }

    printf("Loaded Spritesheet Animation: %s\n", animation->filename);

    goto cleanup;

error:
    Animation_cleanup(animation);
cleanup:
    cJSON_Delete(root);
}

#undef GET_ARRAY
#undef GET_STRING
#undef GET_FLOAT

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