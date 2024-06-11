#include "raylib.h"
#include "rlgl.h"
#include "../g.h"
#include "shared/scene_graph/scene_graph.h"

void SpritesheetAnimatorComponent_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                         float delta, void *componentData)
{
    SpritesheetAnimatorComponent *component = (SpritesheetAnimatorComponent *)componentData;
}

BEGIN_COMPONENT_REGISTRATION(SpritesheetAnimatorComponent){
    .updateTick = SpritesheetAnimatorComponent_update,
} END_COMPONENT_REGISTRATION