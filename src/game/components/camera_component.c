#include "game/g.h"
#include "camera_component.h"
#include "shared/scene_graph/scene_graph.h"
#include <raylib.h>

Camera3D CameraComponent_getCamera3D(SceneGraph* sceneGraph, SceneObjectId nodeId)
{
    CameraComponent* cameraComponent;
    SceneComponent* sceneComponent;
    if ((sceneComponent = SceneGraph_getComponentByType(sceneGraph, nodeId, _componentIdMap.CameraComponentId, (void**)&cameraComponent, 0)) == NULL)
    {
        return (Camera3D) {0};
    }

    Vector3 forward = SceneGraph_getWorldForward(sceneGraph, sceneComponent->objectId);
    // Vector3 up = SceneGraph_getWorldUp(sceneGraph, sceneComponent->objectId);
    Vector3 position = SceneGraph_getWorldPosition(sceneGraph, sceneComponent->objectId);
    if (cameraComponent->trackTarget)
    {
        position = Vector3Subtract(cameraComponent->targetPoint, Vector3Scale(forward, cameraComponent->targetDistance));
        SceneGraph_setLocalPosition(sceneGraph, sceneComponent->objectId, position);
    }
    Camera3D camera = cameraComponent->camera;
    camera.position = position;
    camera.target = Vector3Add(position, forward);
    camera.up = (Vector3){0, 1, 0};
    return camera;
}

