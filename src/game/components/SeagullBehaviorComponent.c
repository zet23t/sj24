#include "game/Components.h"
#include "AnimatorComponent.h"
#include "raymath.h"
#include "math.h"

#define DEFINITIONS
#include "TileMapComponent.h"
#undef DEFINITIONS

static void SeagullBehaviorComponent_onInitialize(SceneObject *sceneObject, SceneComponentId sceneComponent,
                                                  void *componentData, void *initData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    *component = *((SeagullBehaviorComponent*) initData);
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
    component->onWaterVarIndex = AnimatorComponent_getVariableIndex(animator, "onWater");
    SeagullBehaviorComponent *init = (SeagullBehaviorComponent *)initData;
    if (init != NULL)
    {
        component->flyHeight = init->flyHeight;
    }
}

static void SeagullBehaviorComponent_stateIdle(SceneObject *sceneObject, SeagullBehaviorComponent *component)
{
    if (component->time < component->nextDecisionTime)
    {
        return;
    }
    Vector3 position = SceneGraph_getWorldPosition(sceneObject->graph, sceneObject->id);
    int index = 0;
    Vector2 target = (Vector2){GetRandomValue(-10, 10) + position.x, GetRandomValue(-10, 10) + position.y};
    float targetDistSq = Vector2DistanceSqr(target, (Vector2){position.x, position.y});
    // find some close seagulls and pick a random one to fly towards
    while (1)
    {
        SeagullBehaviorComponent *other = NULL;
        SceneComponent *otherComponent = SceneGraph_getComponentByType(sceneObject->graph, (SceneObjectId){0}, _componentIdMap.SeagullBehaviorComponentId, (void**)&other, index++);
        if (otherComponent == NULL)
        {
            break;
        }
        if (otherComponent->objectId.id == sceneObject->id.id)
        {
            continue;
        }
        Vector3 otherPosition = SceneGraph_getWorldPosition(sceneObject->graph, otherComponent->objectId);
        float sqDist = Vector2DistanceSqr((Vector2){position.x, position.y}, (Vector2){otherPosition.x, otherPosition.y});
        if (sqDist < targetDistSq && GetRandomValue(0, 10) > 7)
        {
            target = (Vector2){otherPosition.x, otherPosition.y};
            targetDistSq = sqDist;
        }
    }
    component->moveTarget = (Vector2){GetRandomValue(-10, 10) * .1f + target.x, GetRandomValue(-10, 10) * .1f + target.y};
    component->nextDecisionTime = component->time + GetRandomValue(10, 20);
    // component->flyHeight = sinf(component->time * 2.0f);
    // if (component->flyHeight < 0.0f)
    // {
    //     component->flyHeight = 0.0f;
    // }
}

static void SeagullBehaviorComponent_update(SceneObject *sceneObject, SceneComponentId sceneComponent,
                                            float delta, void *componentData)
{
    SeagullBehaviorComponent *component = (SeagullBehaviorComponent *)componentData;
    component->time += delta;

    Vector3 position = SceneGraph_getWorldPosition(sceneObject->graph, sceneObject->id);
    float sqDist = Vector2DistanceSqr((Vector2){position.x, position.y}, component->moveTarget);
    int flip = 0;
    if (sqDist > 0.1f)
    {
        float distance = sqrtf(sqDist);
        Vector2 direction = Vector2Normalize(Vector2Subtract(component->moveTarget, (Vector2){position.x, position.y}));
        position.x += direction.x * delta * 1.0f;
        position.y += direction.y * delta * 1.0f;
        if (direction.x < 0.0f)
        {
            flip = 1;
        }
        else
        {
            flip = -1;
        }
        SceneGraph_setLocalPosition(sceneObject->graph, sceneObject->id, position);
        component->flyHeight += delta * 1.0f;
        float maxHeight = distance > 1.0f ? 1.0f : distance;
        if (component->flyHeight > maxHeight)
        {
            component->flyHeight = maxHeight;
        }
    }
    else
    {
        component->flyHeight -= delta * 2.0f;
        if (component->flyHeight < 0.0f)
        {
            component->flyHeight = 0.0f;
        }
    }

    SeagullBehaviorComponent_stateIdle(sceneObject, component);

    SceneGraph_setLocalPosition(sceneObject->graph, component->sprite, (Vector3){0.0f, component->flyHeight * 1.0f, 0.0f});

    SpriteRendererComponent *spriteRenderer;
    if (SceneGraph_getComponentByType(sceneObject->graph, component->sprite, _componentIdMap.SpriteRendererComponentId,
                                      (void **)&spriteRenderer, 0))
    {
        spriteRenderer->sortId = -(int)(position.y - component->flyHeight * 8.0f);
        if (flip > 0)
        {
            spriteRenderer->flip.x = 1;
        }
        else if (flip < 0)
        {
            spriteRenderer->flip.x = -1;
        }
    }

    SpriteRendererComponent *shadowSprite;
    if (SceneGraph_getComponent(sceneObject->graph, component->shadow, (void**) &shadowSprite))
    {
        int alpha = 255 - (int)(component->flyHeight * 255.0f);
        alpha = alpha & 0b11000000;
        if (alpha < 64)
        {
            alpha = 64;
        }
        shadowSprite->tint.a = (uint8_t)alpha;
    }

    AnimatorComponent *animator;
    SceneGraph_getComponent(sceneObject->graph, component->animator, (void **)&animator);
    AnimatorComponent_setVariableValue(animator, component->flyHeightVarIndex, component->flyHeight);

    float onWater = 1.0f;
    TileMapComponent *tileMap;
    SceneComponent *tileMapComponent = SceneGraph_getComponentByType(sceneObject->graph, (SceneObjectId){0}, _componentIdMap.TileMapComponentId, (void**)&tileMap, 0);
    if (tileMapComponent)
    {
        int type = TileMapComponent_getTileTypeAtWorldPosition(sceneObject->graph, tileMapComponent->objectId, tileMapComponent->id, tileMap, position, 0);
        if (type > 1)
        {
            onWater = 0.0f;
        }
    }
    AnimatorComponent_setVariableValue(animator, component->onWaterVarIndex, onWater);
}

BEGIN_COMPONENT_REGISTRATION(SeagullBehaviorComponent){
    .onInitialize = SeagullBehaviorComponent_onInitialize,
    .updateTick = SeagullBehaviorComponent_update,
} END_COMPONENT_REGISTRATION