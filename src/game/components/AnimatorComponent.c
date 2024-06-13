#include "raylib.h"
#include "rlgl.h"
#include "../g.h"
#include "shared/scene_graph/scene_graph.h"

void AnimatorComponent_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                         float delta, void *componentData)
{
    AnimatorComponent *component = (AnimatorComponent *)componentData;
}

BEGIN_COMPONENT_REGISTRATION(AnimatorComponent){
    .updateTick = AnimatorComponent_update,
} END_COMPONENT_REGISTRATION