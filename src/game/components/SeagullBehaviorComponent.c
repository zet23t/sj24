#include "game/Components.h"
#include "AnimatorComponent.h"
#include "raymath.h"
#include "math.h"

static void SeagullBehaviorComponent_onInitialize(SceneObject *sceneObject, SceneComponentId sceneComponent,
                                                  void *componentData, void *initData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    component->flyHeight = 0.0f;
    component->flyHeightVarIndex = -1;
    component->animator = (SceneComponentId){0};
    component->time = 0.0f;
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

    component->flyHeight = sinf(component->time * 2.0f) + 0.5f;
    component->time += delta;

    SpriteRendererComponent *spriteRenderer;
    if (SceneGraph_getComponentByType(sceneObject->graph, sceneObject->id, _componentIdMap.SpriteRendererComponentId,
                                      (void **)&spriteRenderer, 0))
    {
        spriteRenderer->sortId = -(int)(sceneObject->transform.position.y - component->flyHeight * 8.0f);
    }

    AnimatorComponent *animator;
    SceneGraph_getComponent(sceneObject->graph, component->animator, (void **)&animator);
    AnimatorComponent_setVariableValue(animator, component->flyHeightVarIndex, component->flyHeight);
}

BEGIN_COMPONENT_REGISTRATION(SeagullBehaviorComponent){
    .onInitialize = SeagullBehaviorComponent_onInitialize,
    .updateTick = SeagullBehaviorComponent_update,
} END_COMPONENT_REGISTRATION