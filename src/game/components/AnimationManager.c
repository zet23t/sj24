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
#define GET_ARRAY(obj, name)                                                                                 \
    cJSON *name = cJSON_GetObjectItem(obj, #name);                                                           \
    if (!cJSON_IsArray(name))                                                                                \
    {                                                                                                        \
        TraceLog(LOG_ERROR, "Failed to load Animation: %s (%d)", animation->filename, __LINE__); \
        goto cleanup;                                                                                        \
    }

    GET_ARRAY(root, clips);


#undef GET_ARRAY

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
    animation->frames_count = 0;
    animation->frames = NULL;
    animation->frames_capacity = 0;
    animation->lastModifiedTime = GetFileModTime(filename);
    animation->filename = filename;
    animation->generation = ++manager->generationCounter;
    Animation_load(animation);
    return (AnimationId){manager->animations_count - 1, animation->generation};
}

static void AnimationManager_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                               float delta, void *componentData)
{
    AnimationManager *component = (AnimationManager *)componentData;
    for (int i = 0; i < component->animations_count; i++)
    {
        Animation *animation = &component->animations[i];
        long modTime = GetFileModTime(animation->filename);
        if (modTime > animation->lastModifiedTime)
        {
            animation->lastModifiedTime = modTime;
            Animation_load(animation);
        }
    }
}

BEGIN_COMPONENT_REGISTRATION(AnimationManager){
    .updateTick = AnimationManager_update,
} END_COMPONENT_REGISTRATION