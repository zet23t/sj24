#include "../g.h"
#define DEFINITIONS
#include "playmap_component.h"
#undef DEFINITIONS

extern Texture2D _spriteSheet;

int PlayMapComponent_calcWhirlScore(PlayMapComponent *playMap, int x, int y)
{
    int seed = ((x * 73856093) ^ (y * 19349663)) % 10000017;
    seed = seed ^ (seed >> 14) ^ ((x * 19 + y * 17) % 14281);
    return seed % 100;
}

TileType PlayMapComponent_getTileType(PlayMapComponent *playMap, int x, int y)
{
    if (x < 0)
    {
        return (TileType){.rotation = 0, .type = TILE_TYPE_CURRENT};
    }
    if (x >= playMap->width)
    {
        return (TileType){.rotation = 2, .type = TILE_TYPE_CURRENT};
    }
    if (y < 0)
    {
        return (TileType){.rotation = 1, .type = TILE_TYPE_CURRENT};
    }
    if (y >= playMap->height)
    {
        return (TileType){.rotation = 3, .type = TILE_TYPE_CURRENT};
    }

    // whirl placement
    // for now, let's only have whirls in even grid coordinates,
    // it's quite complex to have whirls in odd grid coordinates due to the overlapping
    int whirlsScore = PlayMapComponent_calcWhirlScore(playMap, x / 2, y / 2);
    if (whirlsScore > 90)
    {
        // non offsetted whirl
        int dx = x & 1;
        int dy = y & 1;
        return (TileType){.rotation = (dx + (dy * dx + (1 - dx) * dy * 3)) % 4, .type = TILE_TYPE_WHIRL};
    }
    whirlsScore = PlayMapComponent_calcWhirlScore(playMap, (x + 17) / 2, (y + 11) / 2);
    if (whirlsScore > 90)
    {
        int dx = (x + 1) & 1;
        int dy = (y + 1) & 1;
        return (TileType){.rotation = (dx + (dy * dx + (1 - dx) * dy * 3)) % 4, .type = TILE_TYPE_WHIRL};
    }

    int seed = ((x * 73856093) ^ (y * 19349663)) % 10000007;
    seed = seed ^ (seed >> 14) ^ ((x * 19 + y * 17) % 14281);

    int rotation = (seed & 3);
    int type = (seed >> 2) % 5 > 3 ? 1 : 0;

    return (TileType){.rotation = rotation, .type = type};
}

static void PlayMapComponent_draw(Camera3D camera, SceneObject *node, SceneComponentId sceneComponentId, void *component, void *userdata)
{
    PlayMapComponent *playMap = (PlayMapComponent *)component;
    if (playMap->mesh.vaoId == 0)
    {
        float *allVertices = malloc(sizeof(float) * 3 * 4 * playMap->width * playMap->height * 2);
        float *allNormals = malloc(sizeof(float) * 3 * 4 * playMap->width * playMap->height * 2);
        float *allTexcoords = malloc(sizeof(float) * 2 * 4 * playMap->width * playMap->height * 2);
        unsigned short *allIndices = malloc(sizeof(unsigned short) * 6 * playMap->width * playMap->height * 2);

        for (int pass = 0; pass < 2; pass += 1)
        {
            float *vertices = &allVertices[pass * 4 * 3 * playMap->width * playMap->height];
            float *normals = &allNormals[pass * 4 * 3 * playMap->width * playMap->height];
            float *texcoords = &allTexcoords[pass * 4 * 2 * playMap->width * playMap->height];
            unsigned short *indices = &allIndices[pass * 6 * playMap->width * playMap->height];

            float u0 = 0;
            float v0 = 16.0f / _spriteSheet.height;
            float u1 = 32.0f / _spriteSheet.width;
            // float v1 = 48.0f / _spriteSheet.height;
            if (pass == 1)
            {
                v0 = 224.0f / _spriteSheet.height;
                // v1 = 256.0f / _spriteSheet.height;
            }
            float z = pass * 0.0f;

            float step = 32.0f / _spriteSheet.height;

            for (int x = 0; x < playMap->width; x++)
            {
                for (int y = 0; y < playMap->height; y++)
                {
                    TileType type = PlayMapComponent_getTileType(playMap, x, y);
                    int typeIndex = type.type;
                    int typeRotation = type.rotation;
                    float vt0 = v0 + typeIndex * step * (1 - pass);
                    float vt1 = vt0 + step;
                    int i = (y * playMap->width + x) * 4;
                    vertices[i * 3] = x;
                    vertices[i * 3 + 1] = z;
                    vertices[i * 3 + 2] = y;
                    vertices[i * 3 + 3] = x + 1;
                    vertices[i * 3 + 4] = z;
                    vertices[i * 3 + 5] = y;
                    vertices[i * 3 + 6] = x + 1;
                    vertices[i * 3 + 7] = z;
                    vertices[i * 3 + 8] = y + 1;
                    vertices[i * 3 + 9] = x;
                    vertices[i * 3 + 10] = z;
                    vertices[i * 3 + 11] = y + 1;
                    normals[i * 3] = 0;
                    normals[i * 3 + 1] = 1;
                    normals[i * 3 + 2] = 0;
                    normals[i * 3 + 3] = 0;
                    normals[i * 3 + 4] = 1;
                    normals[i * 3 + 5] = 0;
                    normals[i * 3 + 6] = 0;
                    normals[i * 3 + 7] = 1;
                    normals[i * 3 + 8] = 0;
                    normals[i * 3 + 9] = 0;
                    normals[i * 3 + 10] = 1;
                    normals[i * 3 + 11] = 0;
                    int rot = typeRotation * 2 * (1 - pass);
                    texcoords[i * 2 + ((0 + rot) & 7)] = u0;
                    texcoords[i * 2 + ((1 + rot) & 7)] = vt0;
                    texcoords[i * 2 + ((2 + rot) & 7)] = u1;
                    texcoords[i * 2 + ((3 + rot) & 7)] = vt0;
                    texcoords[i * 2 + ((4 + rot) & 7)] = u1;
                    texcoords[i * 2 + ((5 + rot) & 7)] = vt1;
                    texcoords[i * 2 + ((6 + rot) & 7)] = u0;
                    texcoords[i * 2 + ((7 + rot) & 7)] = vt1;
                }
            }
            unsigned short indexOffset = pass * 4 * playMap->width * playMap->height;
            for (int x = 0; x < playMap->width; x++)
            {
                for (int y = 0; y < playMap->height; y++)
                {
                    int i = (y * playMap->width + x) * 6;
                    unsigned short vi = (unsigned short)(indexOffset + (y * playMap->width + x) * 4);
                    indices[i] = vi;
                    indices[i + 1] = vi + 2;
                    indices[i + 2] = vi + 1;
                    indices[i + 3] = vi;
                    indices[i + 4] = vi + 3;
                    indices[i + 5] = vi + 2;
                }
            }
        }

        playMap->mesh.vertexCount = 4 * playMap->width * playMap->height * 2;
        playMap->mesh.vertices = allVertices;
        playMap->mesh.normals = allNormals;
        playMap->mesh.texcoords = allTexcoords;
        playMap->mesh.indices = allIndices;
        playMap->mesh.triangleCount = 2 * playMap->width * playMap->height * 2;
        UploadMesh(&playMap->mesh, false);

        Material material = {0};
        material = LoadMaterialDefault();
        playMap->material = material;
        playMap->material.maps[MATERIAL_MAP_DIFFUSE].texture = _spriteSheet;
    }
    DrawMesh(playMap->mesh, playMap->material, SceneObject_getWorldMatrix(node));
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){1, 0, 0}, RED);
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 1}, GREEN);
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 1, 0}, BLUE);
    // for (int x = 0; x < playMap->width; x++)
    // {
    //     for (int y = 0; y < playMap->height; y++)
    //     {
    //         Vector3 pos = (Vector3){x, 0, y};
    //         DrawCube(pos, .95f, 0, .95f, (Color){64,64,96,64});
    //     }
    // }
    // DrawGrid(playMap->width, 1);
    // DrawCube((Vector3){0, 0, 0}, 1, 1, 1, RED);
    // PlayMapComponent* playMap = (PlayMapComponent*)component;
    // Matrix m = SceneObject_getWorldMatrix(node);
    // DrawMesh(*playMap->mesh, *playMap->material, m);
}

BEGIN_COMPONENT_REGISTRATION(PlayMapComponent){
    .draw = PlayMapComponent_draw,
} END_COMPONENT_REGISTRATION