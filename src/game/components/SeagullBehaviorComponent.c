#include "game/Components.h"

static void SeagullBehaviorComponent_update(SceneObject *sceneObject, SceneComponentId SceneComponent,
                                    float delta, void *componentData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    // AnimatorComponent *animator = SceneObject_getComponent(sceneObject->id, AnimatorComponent);
}


BEGIN_COMPONENT_REGISTRATION(SeagullBehaviorComponent) {
    .updateTick = SeagullBehaviorComponent_update,
} END_COMPONENT_REGISTRATION