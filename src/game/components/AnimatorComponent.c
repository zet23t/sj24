#define DEFINITIONS
#include "raylib.h"
#include "rlgl.h"
#include "../g.h"
#include "shared/scene_graph/scene_graph.h"
#include "AnimationManager.h"
#include "shared/serialization/reflection.h"
#undef DEFINITIONS

void AnimatorComponent_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                         float delta, void *componentData)
{
    AnimatorComponent *component = (AnimatorComponent *)componentData;
    if (component->animationId.generation == 0)
    {
        return;
    }

    AnimationManager *animationManager = AnimationManager_getInstance(sceneObject->graph);
    Animation *animation = AnimationManager_getAnimationById(animationManager, component->animationId);
    if (animation == NULL)
    {
        return;
    }

    component->currentTime += delta;
    if (animation->clips_count == 0)
    {
        return;
    }
    AnimationClip *clip = &animation->clips[0];
    float time = fmod(component->currentTime, clip->duration);
    for (int i = 0; i < clip->tracks_count; i++)
    {
        AnimationTrack *track = &clip->tracks[i];
        float value = 0;
        for (int j = 0; j < track->keys_count; j++)
        {
            AnimationKey *key = &track->keys[j];
            if (key->time > time)
            {
                break;
            }
            value = key->value;
        }
        float *floatValue = NULL;
        size_t size = 0;
        const char *type = NULL;
        int result = SceneGraph_retrieve(sceneObject->graph, sceneObject->id, track->path, (void**)&floatValue, &size, &type);
        if (result == REFLECT_OK)
        {
            *floatValue = value;
        }
    }
}

BEGIN_COMPONENT_REGISTRATION(AnimatorComponent){
    .updateTick = AnimatorComponent_update,
} END_COMPONENT_REGISTRATION