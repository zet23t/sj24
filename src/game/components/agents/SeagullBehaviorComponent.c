#include "raylib.h"
#include "rlgl.h"
#include "../../g.h"
#include "shared/scene_graph/scene_graph.h"
#define DEFINITIONS
#include "SeagullBehaviorComponent.h"
#include "../AnimatorComponent.h"
#undef DEFINITIONS
#include "shared/serialization/serializable_structs.h"

static void SeagullBehaviorComponent_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                    float delta, void *componentData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    // AnimatorComponent *animator = SceneObject_getComponent(sceneObject->id, AnimatorComponent);
}


BEGIN_COMPONENT_REGISTRATION(SeagullBehaviorComponent) {
    .updateTick = SeagullBehaviorComponent_update,
} END_COMPONENT_REGISTRATION