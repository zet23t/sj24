#define DEFINITIONS
#include "game/Components.h"
#include "shared/scene_graph/scene_graph.h"
#include "shared/serialization/serializable_structs.h"
#include "g.h"
#include "components/camera_component.h"
#include "components/playmap_component.h"
#include "components/TileMapComponent.h"
#include <rlgl.h>
#undef DEFINITIONS

#define BEACH_START 4.5f
#define BEACH_END 6.0f

static void spawnSeagull(SceneGraph *g, Vector3 position)
{
    // printf("Spawning seagull at %.2f %.2f %.2f\n", position.x, position.y, position.z);
    SceneObjectId seagull = SceneGraph_createObject(g, "seagull");
    SceneGraph_setLocalPosition(g, seagull, position);
    SceneObjectId seagullSprite = SceneGraph_createObject(g, "sprite");
    SceneGraph_setParent(g, seagullSprite, seagull);
    SceneGraph_addComponent(g, seagullSprite, _componentIdMap.SpriteRendererComponentId,
                            &(SpriteRendererComponent){
                                .spriteAsset = (SpriteAsset){
                                    .texture = _spriteSheet,
                                    .source = (Rectangle){.x = 0, .y = 656, .width = 8, .height = 8},
                                },
                                .snapping = 1.0f/16.0f,
                                .size = (Vector2){0.5f, 0.5f},
                                .tint = WHITE,
                                .pivot = (Vector2){0.5f, 6.0f/8.0f},
                                .pixelsPerUnit = 16,
                                .flip.x = GetRandomValue(0, 1) * 2.0f - 1.0f,
                                .sortId = -(int)position.y
                            });
    AnimationManager* mgr = AnimationManager_getInstance(g);
    AnimatorVariable variables[2];
    variables[0].name = "flyHeight";
    variables[0].value = 0.0f;
    variables[1].name = "onWater";
    variables[1].value = 0.0f;
    SceneGraph_addComponent(g, seagull, _componentIdMap.AnimatorComponentId, &(AnimatorComponent){
        .animationId = AnimationManager_getAnimation(mgr, "assets/seagull_animation.anim"),
        .animationName = "idle",
        .currentTime = GetRandomValue(0, 100) * 0.001f,
        .loopCount = 0,
        .variables_capacity = 2,
        .variables_count = 2,
        .variables = variables,
    });

    SceneObjectId shadow = SceneGraph_createObject(g, "shadow");
    SceneGraph_setParent(g, shadow, seagull);
    SceneGraph_setLocalPosition(g, shadow, (Vector3){0.0f, 0.0f, 0.0f});
    SceneComponentId shadowComponent = SceneGraph_addComponent(g, shadow, _componentIdMap.SpriteRendererComponentId, &(SpriteRendererComponent){
        .spriteAsset = (SpriteAsset){
            .texture = _spriteSheet,
            .source = (Rectangle){.x = 0, .y = 664, .width = 8, .height = 8},
        },
        .snapping = 1.0f/16.0f,
        .size = (Vector2){0.5f, 0.5f},
        .tint = WHITE,
        .pivot = (Vector2){0.5f, 6.0f/8.0f},
        .pixelsPerUnit = 16,
        .flip.x = GetRandomValue(0, 1) * 2.0f - 1.0f,
        .sortId = -10000 - (int)position.y
    });

    SceneGraph_addComponent(g, seagull, _componentIdMap.SeagullBehaviorComponentId, &(SeagullBehaviorComponent){
        .flyHeight = GetRandomValue(0, 10000) * 0.0001f,
        .shadow = shadowComponent,
        .sprite = seagullSprite,
        .time = GetRandomValue(0, 10000) * 0.001f,
    });

    // printf("Seagull created\n");

    // SceneGraph_addComponent(g, seagull, _componentIdMap.PrimitiveRendererComponentId, &(PrimitiveRendererComponent){
    //     .primitiveType = PRIMITIVE_TYPE_CUBE,
    //     .size = (Vector3){1.0f, 1.0f, 1.0f},
    //     .color = (Color){255, 0, 0, 255},
    //     .isWireframe = 1,
    // });
}

static void trySpawnGrass(SceneGraph *g, int x, int y, int type, float value, Vector2 position)
{
    if (type != 4 || GetRandomValue(0, 100) > 60)
    {
        return;
    }
    int grassType = GetRandomValue(0, 3);
    SceneObjectId plant = SceneGraph_createObject(g, "plant");
    SceneGraph_setLocalPosition(g, plant, (Vector3){position.x, position.y + 0.5f, 0.1f});
    SceneGraph_addComponent(g, plant, _componentIdMap.SpriteRendererComponentId,
                            &(SpriteRendererComponent){
                                .spriteAsset = (SpriteAsset){
                                    .texture = _spriteSheet,
                                    .source = (Rectangle){.x = 16 * grassType, .y = _spriteSheet.height - 48, .width = 16, .height = 16},
                                },
                                .size = (Vector2){1, 1},
                                .tint = WHITE,
                                .pivot = (Vector2){0.5f, 12.0f/16.0f},
                                .pixelsPerUnit = 16,
                                .sortId = -(int)position.y - 18
                            });
}
static void trySpawnTree(SceneGraph *g, int x, int y, int type, float value, Vector2 position)
{
    if (type != 4 || GetRandomValue(0, 100) > 60)
    {
        return;
    }
    
    int treeType = GetRandomValue(0, 2);
    float diffused = value + (GetRandomValue(0, 10000) * 0.0001f - .5f) * 8.0f;
    if (diffused < 8.55f)
    {
        treeType = GetRandomValue(5,6);
    }
    else if (diffused < 10.5f)
    {
        treeType = GetRandomValue(3,4);
    }
    else if (diffused < 12.0f)
    {
        treeType = GetRandomValue(0,3);
    }
    SceneObjectId plant = SceneGraph_createObject(g, "plant");
    SceneGraph_setLocalPosition(g, plant, (Vector3){position.x, position.y + 1.0f, 0.1f});
    SceneGraph_addComponent(g, plant, _componentIdMap.SpriteRendererComponentId,
                            &(SpriteRendererComponent){
                                .spriteAsset = (SpriteAsset){
                                    .texture = _spriteSheet,
                                    .source = (Rectangle){.x = 16 * treeType, .y = _spriteSheet.height - 32, .width = 16, .height = 32},
                                },
                                .size = (Vector2){1, 2},
                                .tint = WHITE,
                                .pivot = (Vector2){0.5f, 12.0f/16.0f},
                                .pixelsPerUnit = 16,
                                .sortId = -(int)position.y
                            });
    // SceneGraph_addComponent(g, plant, _componentIdMap.PrimitiveRendererComponentId, &(PrimitiveRendererComponent){
    //     .primitiveType = PRIMITIVE_TYPE_CUBE,
    //     .size = (Vector3){.1f, .1f, 0.1f},
    //     .color = (Color){0, 255, 0, 255},
    //     .isWireframe = 1,
    // });
}

static void trySpawnBeach(SceneGraph *g, int x, int y, int type, float value, Vector2 position)
{
    if (type != 3)
    {
        return;
    }
    else if (GetRandomValue(0, 100) > 20 + (value-BEACH_START)/(BEACH_END-BEACH_START) * 50.0f)
    {
        if (GetRandomValue(0,100) > 90)
        {
            spawnSeagull(g, (Vector3){position.x, position.y + 0.5f, 0.1f});
        }
        return;
    }
    SceneObjectId plant = SceneGraph_createObject(g, "beach");
    SceneGraph_setLocalPosition(g, plant, (Vector3){position.x, position.y + 0.5f, 0.1f});
    SceneGraph_addComponent(g, plant, _componentIdMap.SpriteRendererComponentId,
                            &(SpriteRendererComponent){
                                .spriteAsset = (SpriteAsset){
                                    .texture = _spriteSheet,
                                    .source = (Rectangle){.x = GetRandomValue(4,6)*16, .y = _spriteSheet.height - 48 - GetRandomValue(0,1)*16, .width = 16, .height = 16},
                                },
                                .size = (Vector2){1, 1},
                                .tint = WHITE,
                                .pivot = (Vector2){0.5f, 12.0f/16.0f},
                                .pixelsPerUnit = 16,
                                .sortId = -(int)position.y - 3
                            });
}

void start_gameScene(SceneGraph *g)
{
    _spriteSheet = ResourceManager_loadTexture(_resourceManager, "assets/tileset.png", TEXTURE_FILTER_POINT);
    SceneGraph_clear(g);
    SceneObjectId systemsId = SceneGraph_createObject(g, "systems");
    TileMapComponent map = {
        .width = 128,
        .height = 128,
        .tileWidth = 16,
        .tileHeight = 16,
        .tileset = _spriteSheet,
        .layerCount = 3,
    };
    map.chunks = malloc(sizeof(TileDataChunk) * 1);
    map.chunks[0] = (TileDataChunk){
        .x = 0,
        .y = 0,
        .width = map.width,
        .height = map.height,
        .layerCount = 3,
        .data = MemAlloc(map.width * map.height * 3),
    };
    unsigned char *data = map.chunks[0].data;
    int rangeX = map.width / 5;
    int rangeY = map.height / 5;
    int areaX = map.width - rangeX * 2;
    int areaY = map.height - rangeY * 2;
    Vector4 points[0xff];
    float values[128*128];

    int pointCount = (int)fminf(sizeof(points) / sizeof(Vector4), (int)(sqrtf(areaX * areaX + areaY * areaY) * 0.25f));
    printf("pointCount: %d %d %d\n", pointCount, rangeX, rangeY);
    for (int i=0;i<pointCount;i++) {
        points[i] = (Vector4){
            GetRandomValue(rangeX, map.width - rangeX), 
            GetRandomValue(rangeY, map.height - rangeY), 
            2.5f + GetRandomValue(0,0xffff) % 7,
            (GetRandomValue(0,100) * 0.01f) * .1f + 0.25f};
        printf("point: %d %.2f %.2f %.2f\n", i, points[i].x, points[i].y, points[i].z);
    }

    for (int x = 0; x < map.width; x++)
    {
        for (int y = 0; y < map.height; y++)
        {
            float nearestD = 1000000;
            float sum = 0;
            float nearestSum = 0;
            float centerD = sqrtf(powf(x - map.width * 0.5f, 2) + powf(y - map.height * 0.5f, 2));
            for (int i = 0; i < pointCount; i++)
            {
                float dx = (x - points[i].x);
                float dy = (y - points[i].y);
                float d = powf(dx * dx + dy * dy, points[i].w);
                float id = points[i].z - d;
                if (id < 0.0f)
                {
                    id = 0.0f;
                }
                sum += id;
                nearestSum += points[i].z;
                if (d < nearestD)
                {
                    nearestD = d;
                }
            }
            float d = sum - centerD * 0.1f;
            
            int index = y * map.width + x;
            values[index] = d;
            int layerIndex = index * 3;
            if (d <= 0.15f)
            {
                data[layerIndex + 0] = 0;
            }
            else if (d < 4.5f)
            {
                data[layerIndex + 0] = 1;
            }
            else if (d < 6.0f)
            {
                data[layerIndex + 0] = 3;
            }
            else
            {
                data[layerIndex + 0] = 4;
            }
        }
    }
    for (int x = 1; x < map.width-1; x++)
    {
        for (int y = 1; y < map.height-1; y++)
        {
            int i00 = y * map.width + x;
            int i10 = i00 + 1;
            int i01 = i00 + map.width;
            int i11 = i01 + 1;
            float d00 = values[i00];
            float d10 = values[i10];
            float d01 = values[i01];
            float d11 = values[i11];
            float avg = (d00 + d10 + d01 + d11) * 0.25f;
            float min = d00 < d10 && d00 < d01 && d00 < d11 ? d00 :
                (d10 < d01 && d10 < d11 ? d10 : (d01 < d11 ? d01 : d11));
            float max = d00 > d10 && d00 > d01 && d00 > d11 ? d00 :
                (d10 > d01 && d10 > d11 ? d10 : (d01 > d11 ? d01 : d11));
            if ((int)avg != 11 && !(min < 11.0f && max > 11.0f))
            {
                continue;
            }

            float dx = (d10 - d00) * 0.5f;
            float dy = (d01 - d00) * 0.5f;
            float slope = sqrtf(dx * dx + dy * dy);
            if (slope < 0.25f)
            {
                continue;
            }
            // float absDx = fabsf(dx);
            // float absDy = fabsf(dy);
            data[i00 * 3 + 2] = dx < 0.0f ? 12 : 13;
            data[i00 * 3 + 1] = dy < 0.0f ? 10 : 11;
        }
    }


    map.chunkCount = 1;
    SceneGraph_addComponent(g, systemsId, _componentIdMap.TileMapComponentId, &map);
    SceneGraph_setLocalPosition(g, systemsId, (Vector3){-map.width * 0.5f, map.height * 0.5f, 0.0f});
    SceneObjectId camera = SceneGraph_createObject(g, "camera");
    SceneGraph_setLocalPosition(g, camera, (Vector3){5, 10, 5});
    SceneGraph_setLocalRotation(g, camera, (Vector3){0.0f, 180.0f, 0.0f});
    SceneGraph_addComponent(g, camera, _componentIdMap.CameraComponentId,
                            &(CameraComponent){
                                .trackTarget = 1,
                                .targetDistance = 16.0f,
                                .targetPoint = (Vector3){5, 0, 5},
                                .camera = (Camera3D){
                                    .fovy = 120.0f,
                                    .projection = CAMERA_ORTHOGRAPHIC,
                                    .near = 1.0f,
                                    .far = 64.0f,
                                }});

    printf("Creating world objects\n");
    spawnSeagull(g, (Vector3){25.0f, 0.0f, 0.1f});    

    for (int y = map.height - 1; y > 0; y--) {
        for (int x = 0; x < map.width; x++)
        {
            int type = TileMapComponent_getTileType(&map, x, y, 0);
            int type1 = TileMapComponent_getTileType(&map, x, y, 1);
            int type2 = TileMapComponent_getTileType(&map, x, y, 2);
            if (type1 != 0 || type2 != 0 || type == 0)
            {
                continue;
            }
            float value = values[(map.height - y - 1) * map.width + x];
            Vector2 position = (Vector2){x - GetRandomValue(-8,8) / 16.0f, y - GetRandomValue(-8,8) / 16.0f};
            position.x -= map.width * 0.5f;
            position.y -= map.height * 0.5f;
            trySpawnGrass(g, x, y, type, value, position);
            trySpawnTree(g, x, y, type, value, position);
            trySpawnBeach(g, x, y, type, value, position);
            // if (SceneGraph_getComponentTypeCount(_scene, _componentIdMap.SpriteRendererComponentId) > 100)
            // {
            //     printf("SpriteRendererComponent count: %d\n", SceneGraph_getComponentType(_scene, _componentIdMap.SpriteRendererComponentId)->components_count);
            //     return;
            // }
        }
    }


    int animatorCount = SceneGraph_getComponentType(_scene, _componentIdMap.AnimatorComponentId)->components_count;
    printf("Animator count: %d\n", animatorCount);
}

#define COMPONENT(T) void T##_register(SceneGraph *scene);
#include "component_list.h"
#undef COMPONENT

int game_init()
{
    rlSetCullFace(RL_CULL_FACE_BACK);
    SetWindowTitle("Summer Jam 2024");
    DuskGui_init();
    Font font = ResourceManager_loadFont(_resourceManager, "assets/myfont-regular.png");
    DuskGui_setDefaultFont(font, font.baseSize, 1);

    _scene = SceneGraph_create();
#define COMPONENT(T) T##_register(_scene);
#include "component_list.h"
#undef COMPONENT

    start_gameScene(_scene);

    return 0;
}

int RayIntersectsPlane(Ray ray, Vector3 normal, float d, float *out)
{
    float vd = Vector3DotProduct(ray.direction, normal);
    if (vd < -1e-6)
    {
        float v0 = -(Vector3DotProduct(normal, ray.position) + d);
        float t = v0 / vd;
        if (t >= 0)
        {
            *out = t;
            return 1;
        }
    }
    return 0;
}

void game_draw()
{
    CameraComponent *cc;
    if (SceneGraph_getComponentByType(_scene, (SceneObjectId){0}, _componentIdMap.CameraComponentId, (void **)&cc, 0) == NULL)
    {
        printf("No camera component found\n");
        return;
    }
    Camera3D camera = CameraComponent_getCamera3D(_scene, (SceneObjectId){0});

    if (DuskGui_dragArea((DuskGuiParams){
            .rayCastTarget = 1,
            .text = "##map-drag",
            .bounds = (Rectangle){.x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight()}}) != 0)
    {
        Vector2 mouseDelta = GetMouseDelta();
        Vector2 mousePosition = GetMousePosition();
        Vector2 prevMousePosition = Vector2Subtract(mousePosition, mouseDelta);
        Ray ray0 = GetScreenToWorldRay(prevMousePosition, camera);
        Ray ray1 = GetScreenToWorldRay(mousePosition, camera);
        Vector3 planeNormal = (Vector3){0, 0, 1};
        float d = 0;
        // printf("Dragging ray1: %.2f %.2f %.2f -> %.2f %.2f %.2f\n",
        //     ray0.position.x, ray0.position.y, ray0.position.z,
        //     ray0.direction.x, ray0.direction.y, ray0.direction.z);
        if (RayIntersectsPlane(ray0, planeNormal, d, &d) && RayIntersectsPlane(ray1, planeNormal, d, &d))
        {
            Vector3 p0 = Vector3Add(ray0.position, Vector3Scale(ray0.direction, d));
            Vector3 p1 = Vector3Add(ray1.position, Vector3Scale(ray1.direction, d));
            Vector3 delta = Vector3Subtract(p0, p1);
            cc->targetPoint = Vector3Add(cc->targetPoint, delta);
        }
    }

    if (DuskGui_getLastEntry()->isHovered)
    {
        float wheel = GetMouseWheelMove();
        if (wheel != 0)
        {
            Vector2 mousePosition = GetMousePosition();
            Ray ray0 = GetScreenToWorldRay(mousePosition, camera);
            cc->camera.fovy = cc->camera.fovy * (1.0f - wheel * .1f);
            if (cc->camera.fovy < 1.0f)
            {
                cc->camera.fovy = 1.0f;
            }
            if (cc->camera.fovy > 120.0f)
            {
                cc->camera.fovy = 120.0f;
            }
            camera.fovy = cc->camera.fovy;
            Ray ray1 = GetScreenToWorldRay(mousePosition, camera);
            Vector3 planeNormal = (Vector3){0, 0, 1};
            float d0 = 0, d1 = 0;
            if (RayIntersectsPlane(ray0, planeNormal, d0, &d0) && RayIntersectsPlane(ray1, planeNormal, d1, &d1))
            {
                Vector3 p0 = Vector3Add(ray0.position, Vector3Scale(ray0.direction, d0));
                Vector3 p1 = Vector3Add(ray1.position, Vector3Scale(ray1.direction, d1));
                Vector3 delta = Vector3Subtract(p0, p1);
                cc->targetPoint = Vector3Add(cc->targetPoint, delta);
            }
        }
    }

    camera = CameraComponent_getCamera3D(_scene, (SceneObjectId){0});
    BeginMode3D(camera);
    SceneGraph_draw(_scene, camera, NULL);
    SceneGraph_sequentialDraw(_scene, camera, NULL);
    TileMapComponent *tileMap;
    SceneComponent *tileMapComponent = SceneGraph_getComponentByType(_scene, (SceneObjectId){0}, _componentIdMap.TileMapComponentId, (void**)&tileMap, 0);
    // if (tileMapComponent)
    //     for (int x=0;x<tileMap->width;x++)
    //     {
    //         for (int y=0;y<tileMap->height;y++)
    //         {
    //             Vector3 worldPos = (Vector3){x - tileMap->width * 0.5f, y - tileMap->height * 0.5f, 0.0f};
    //             int type = TileMapComponent_getTileTypeAtWorldPosition(_scene, tileMapComponent->objectId, tileMapComponent->id, tileMap, worldPos, 0);
    //             if (type == 0)
    //             {
    //                 continue;
    //             }
    //             Vector3 position = (Vector3){x - tileMap->width * 0.5f, y - tileMap->height * 0.5f, 0.0f};
    //             DrawCube(position, 0.20f, 0.20f, 0.20f, (Color){32 * type, 0, 0, 255});
    //         }
    //     }
    EndMode3D();

    if (IsKeyReleased(KEY_F10))
    {
        _editorState->displayHierarchy = !_editorState->displayHierarchy;
    }

    Editor_draw(_editorState, _scene);
    Editor_drawControls(_editorState, _scene);

    DuskGui_finalize();
}

void game_update(float udt)
{
    ProcessTouches();

    Vector2 mouseDelta = GetMouseDelta();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
    {
        // psg.hasMouse = 1;
    }

    if (IsKeyPressed(KEY_P))
    {
        _editorState->paused = !_editorState->paused;
    }
    if (IsKeyPressed(KEY_N))
    {
        _editorState->stepped = true;
    }
    if (IsKeyPressed(KEY_F2))
    {
        SceneGraph_clear(_scene);
        start_gameScene(_scene);
    }

    float dt = GetFrameTime();
    if (_editorState->paused && !_editorState->stepped)
    {
        dt = 0;
    }
    else
    {
        _editorState->stepped = false;
        _editorState->frameCount++;
    }
    _editorState->gameTime += dt;
    SceneGraph_updateTick(_scene, dt);
}