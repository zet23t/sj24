#include "raylib.h"
#include "rlgl.h"
#include "../g.h"
#include "shared/scene_graph/scene_graph.h"
#include "shared/resource_manager.h"


SpritesheetAnimationManager* SpritesheetAnimationManager_getInstance(SceneGraph *sceneGraph)
{
    SpritesheetAnimationManager *animationManager;
    SceneGraph_getComponentByType(sceneGraph, (SceneObjectId){0}, _componentIdMap.SpritesheetAnimationManagerId, (void **)&animationManager, 0);
    if (animationManager == NULL)
    {
        SceneObjectId id = SceneGraph_createObject(sceneGraph, "SpritesheetAnimationManager");
        SceneComponentId componentId = SceneGraph_addComponent(sceneGraph, id, _componentIdMap.SpritesheetAnimationManagerId, &(SpritesheetAnimationManager){0});
        if (SceneGraph_getComponent(sceneGraph, componentId, (void **)&animationManager) == NULL)
        {
            TraceLog(LOG_ERROR, "Failed to get SpritesheetAnimationManager component");
            return NULL;
        }
    }
    return animationManager;
}

void SpritesheetAnimation_load(SpritesheetAnimation *animation)
{
    printf("Loading Spritesheet Animation: %s\n", animation->filename);

    char *content = ResourceManager_loadText(_resourceManager, animation->filename);
    if (content == NULL)
    {
        TraceLog(LOG_ERROR, "Failed to load Spritesheet Animation: %s", animation->filename);
        return;
    }

    int pos = 0;

    #define SKIP_ALL_WHITESPACE() while (content[pos] && content[pos] <= ' ') pos++
    #define SKIP_WHITESPACE() while (content[pos] && (content[pos] == ' ' || content[pos] == '\t')) pos++
    #define READ_NAME(buffer) for (int i=0;i<sizeof(buffer);i++) buffer[i] = 0; \
    for (int i=0;i<sizeof(buffer)-1;i++){\
        if (content[pos] == '\0' || content[pos] == ' ' || content[pos] == '\t' || content[pos] == '\n')\
        {\
            buffer[i] = '\0';\
            break;\
        }\
        buffer[i] = content[pos++];\
    }

    while (content[pos] != '\0')
    {
        SKIP_ALL_WHITESPACE();
        if (content[pos] == '\0')
            break;
        
        // ignore comments
        if (content[pos] == '#')
        {
            while (content[pos] && content[pos] != '\n')
                pos++;
            continue;
        }

        // state declaration
        if (strncmp(&content[pos], "state:", 6) == 0)
        {
            pos += 6;
            SKIP_WHITESPACE();

            char stateName[128];
            READ_NAME(stateName);
            printf("State: %s\n", stateName);
            continue;
        }

        char token[128];
        READ_NAME(token);
        printf("Unknown token: %s\n", token);
        break;
    }
}

STRUCT_LIST_ACQUIRE_FN(SpritesheetAnimationManager, SpritesheetAnimation, animations)

SpritesheetAnimationId SpritesheetAnimationManager_getAnimation(SpritesheetAnimationManager *manager, char *filename)
{
    for (int i = 0; i < manager->animations_count; i++)
    {
        if (strcmp(manager->animations[i].filename, filename) == 0)
        {
            return (SpritesheetAnimationId){i, manager->animations[i].generation};
        }
    }
    SpritesheetAnimation *animation = SpritesheetAnimationManager_acquire_animations(manager);
    animation->frames_count = 0;
    animation->frames = NULL;
    animation->frames_capacity = 0;
    animation->lastModifiedTime = GetFileModTime(filename);
    animation->filename = filename;
    animation->generation = ++manager->generationCounter;
    SpritesheetAnimation_load(animation);
    return (SpritesheetAnimationId){manager->animations_count - 1, animation->generation};
}

static void SpritesheetAnimationManager_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                              float delta, void *componentData)
{
    SpritesheetAnimationManager *component = (SpritesheetAnimationManager *)componentData;
    for (int i=0;i<component->animations_count;i++){
        SpritesheetAnimation *animation = &component->animations[i];
        long modTime = GetFileModTime(animation->filename);
        if (modTime > animation->lastModifiedTime){
            animation->lastModifiedTime = modTime;
            SpritesheetAnimation_load(animation);
        }
    }
}

BEGIN_COMPONENT_REGISTRATION(SpritesheetAnimationManager){
    .updateTick = SpritesheetAnimationManager_update,
} END_COMPONENT_REGISTRATION