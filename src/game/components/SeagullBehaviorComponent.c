#include "game/Components.h"
#include "AnimatorComponent.h"

static void SeagullBehaviorComponent_onInitialize(SceneObject *sceneObject, SceneComponentId sceneComponent,
                                                  void *componentData, void *initData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    component->flyHeight = 0.0f;
    component->flyHeightVarIndex = -1;
    component->animator = (SceneComponentId){0};
    AnimatorComponent *animator;
    SceneComponent *animatorComponent =
        SceneGraph_getComponentByType(sceneObject->graph, sceneObject->id,
                                      _componentIdMap.AnimatorComponentId, (void **)&animator, 0);
    if (animator == NULL)
    {
        TraceLog(LOG_WARNING, "SeagullBehaviorComponent requires an AnimatorComponent");
        SceneGraph_destroyComponent(sceneObject->graph, sceneComponent);
        return;
    }

    component->animator = animatorComponent->id;
    component->flyHeightVarIndex = AnimatorComponent_getVariableIndex(animator, "flyHeight");
    SeagullBehaviorComponent *init = (SeagullBehaviorComponent *)initData;
    if (init != NULL)
    {
        component->flyHeight = init->flyHeight;
    }
}

static void SeagullBehaviorComponent_update(SceneObject *sceneObject, SceneComponentId sceneComponent,
                                            float delta, void *componentData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    AnimatorComponent *animator;
    SceneGraph_getComponent(sceneObject->graph, component->animator, (void **)&animator);
    AnimatorComponent_setVariableValue(animator, component->flyHeightVarIndex, component->flyHeight);
}

BEGIN_COMPONENT_REGISTRATION(SeagullBehaviorComponent){
    .onInitialize = SeagullBehaviorComponent_onInitialize,
    .updateTick = SeagullBehaviorComponent_update,
} END_COMPONENT_REGISTRATION