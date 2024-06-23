#include "raylib.h"
#include "rlgl.h"
#include "../g.h"

typedef struct TileTypeSpriteLocation
{
    unsigned char x;
    unsigned char y;
    unsigned char width, height;
} TileTypeSpriteLocation;

// a mapping of a pattern of 2x2 vertices to a sprite in the tileset
TileTypeSpriteLocation tileTypeSpriteLocations[] = {
    // 0 0
    // 0 0 (does not exist)
    {0xff, 0xff},
    // 0 0
    // 0 1
    {0, 0},
    // 0 0
    // 1 0
    {3, 0},
    // 0 0
    // 1 1
    {1, 0, 1, 0},
    // 0 1
    // 0 0
    {0, 4},
    // 0 1
    // 0 1
    {0, 1, 0, 2},
    // 0 1
    // 1 0 (does not exist)
    {0xff, 0xff},
    // 0 1
    // 1 1
    {2, 3},
    // 1 0
    // 0 0
    {3, 4},
    // 1 0
    // 0 1 (does not exist)
    {0xff, 0xff},
    // 1 0
    // 1 0
    {3, 1, 0, 2},
    // 1 0
    // 1 1
    {1, 3},
    // 1 1
    // 0 0
    {1, 4, 1, 0},
    // 1 1
    // 0 1
    {2, 2},
    // 1 1
    // 1 0
    {1, 2},
    // 1 1
    // 1 1
    {1, 1, 1, 0}};

// static int ReadInteger(const char *content, int *index)
// {
//     int result = 0;
//     while (content[*index] >= '0' && content[*index] <= '9')
//     {
//         result = result * 10 + content[*index] - '0';
//         (*index)++;
//     }
//     return result;
// }

static int fast_rand(int seed) {
    seed = (214013*seed+2531011);
    return (seed>>16)&0x7FFF;
}

int GetLocationRandom(int x, int y, int z, int max)
{
    unsigned int seed = x * 17 ^ y * 11 ^ (z * 7 + 291123835);
    seed = (((seed % 1221) ^ seed ^ (x * 19)) + y * 31 + z) * 29;
    return fast_rand(seed) % (max + 1);
}

// static int TryReadInteger(const char *content, int *index, int *result)
// {
//     while (content[*index] && content[*index] <= ' ' && content[*index] != '\n')
//         (*index)++;
//     if (content[*index] >= '0' && content[*index] <= '9')
//     {
//         *result = ReadInteger(content, index);
//         return 1;
//     }
//     return 0;
// }

// static int GetLineNumber(const char *content, int at)
// {
//     int line = 1;
//     for (int i = 0; i < at && content[i]; i++)
//     {
//         if (content[i] == '\n')
//             line++;
//     }
//     return line;
// }

// static int LogError(const char *message, const char *content, int at)
// {
//     TraceLog(LOG_WARNING, "%s @ %d", message, GetLineNumber(content, at));
//     return 0;
// }

// static int TileDataChunk_parse(TileDataChunk *chunk, const char *content, int *index)
// {
//     TraceLog(LOG_INFO, "Parsing chunk");
//     *index += 1;
//     if (!TryReadInteger(content, index, &chunk->x))
//         return LogError("Failed to read chunk x", content, *index);

//     if (!TryReadInteger(content, index, &chunk->y))
//         return LogError("Failed to read chunk y", content, *index);

//     int width = 0;
//     int numbersInLineCount = 0;
//     int isNewLine = 1;
//     int layerCount = 0;
//     int posY[26] = {0};
//     int layer = 0;

//     int i = *index;
//     while (content[i])
//     {
//         while (content[i] && content[i] <= ' ' && content[i] != '\n')
//             i++;

//         if (content[i] == '\n')
//         {
//             if (numbersInLineCount > 0)
//             {
//                 if (width == 0)
//                     width = numbersInLineCount;
//                 else if (width != numbersInLineCount)
//                 {
//                     return LogError("Tilemap width mismatch", content, i);
//                 }

//                 posY[layer]++;
//             }
//             numbersInLineCount = 0;
//             isNewLine = 1;
//             i++;
//             continue;
//         }

//         char chr = content[i];
//         if (chr == '#')
//         {
//             while (content[i] && content[i] != '\n')
//                 i++;
//             continue;
//         }
//         if (content[i] == '>' && isNewLine)
//         {
//             break;
//         }
//         if (content[i] >= '0' && content[i] <= '9')
//         {
//             int id;
//             if (!TryReadInteger(content, &i, &id))
//                 return LogError("Failed to read tile id", content, i);
//             numbersInLineCount++;
//             continue;
//         }
//         if (content[i] >= 'A' && content[i] <= 'Z' && isNewLine)
//         {
//             int layerIndex = content[i] - 'A';
//             layerCount = (layerIndex + 1 > layerCount) ? layerIndex + 1 : layerCount;
//             isNewLine = 0;
//             layer = layerIndex;
//             i++;
//         }

//         if (content[i] == '#')
//         {
//             while (content[i] && content[i] != '\n')
//                 i++;
//         }
//     }

//     if (width == 0)
//     {
//         TraceLog(LOG_WARNING, "chunk width is 0");
//         chunk->data = NULL;
//         return 0;
//     }

//     if (chunk->data)
//     {
//         MemFree(chunk->data);
//     }
//     int maxY = 0;
//     for (int i = 0; i < layerCount; i++)
//     {
//         if (posY[i] > maxY)
//             maxY = posY[i];
//         posY[i] = 0;
//     }
//     int height = maxY + 1;
//     chunk->width = width;
//     chunk->height = height;
//     chunk->layerCount = layerCount;
//     chunk->data = MemAlloc(sizeof(unsigned char) * width * height * layerCount);
//     for (int i = 0; i < width * height * layerCount; i++)
//         chunk->data[i] = 0;

//     TraceLog(LOG_INFO, "Reading chunk data, width: %d, height: %d, layerCount: %d", width, height, layerCount);
//     numbersInLineCount = 0;
//     isNewLine = 1;
//     layer = 0;
//     i = *index;
//     while (content[i])
//     {
//         if (content[i] == '\n')
//         {
//             if (numbersInLineCount > 0)
//             {
//                 posY[layer]++;
//             }

//             numbersInLineCount = 0;
//             isNewLine = 1;
//             i++;
//             continue;
//         }

//         if (content[i] == '>' && isNewLine)
//         {
//             break;
//         }

//         if (content[i] >= '0' && content[i] <= '9')
//         {
//             int id = 0;
//             if (!TryReadInteger(content, &i, &id))
//                 return LogError("Failed to read tile id", content, i);

//             int pos = numbersInLineCount * layerCount + layer;
//             chunk->data[posY[layer] * width * layerCount + pos] = id;
//             numbersInLineCount++;
//             isNewLine = 0;
//             continue;
//         }

//         if (content[i] >= 'A' && content[i] <= 'Z')
//         {
//             if (!isNewLine)
//             {
//                 return LogError("Layer declaration must be on a new line", content, i);
//             }
//             layer = content[i] - 'A';
//             isNewLine = 0;
//             i++;
//             continue;
//         }

//         if (content[i] == '#')
//         {
//             while (content[i] && content[i] != '\n')
//                 i++;
//             continue;
//         }

//         i++;
//     }

//     TraceLog(LOG_INFO, "Chunk data reading finished");

//     *index = i;

//     return 1;
// }

// void Tilemap_parse(Tilemap *tilemap, const char *content)
// {
//     int chunkCount = 0;
//     for (int i = 0; content[i]; i++)
//     {
//         while (content[i] && content[i] <= ' ')
//             i++;
//         char chr = content[i];
//         if (chr == '#')
//         {
//             while (content[i] && content[i] != '\n')
//                 i++;
//             continue;
//         }

//         if (chr == '>')
//         {
//             chunkCount++;
//         }

//         while (content[i] && content[i] != '\n')
//             i++;
//     }

//     if (tilemap->chunks)
//     {
//         for (int i = 0; i < tilemap->chunkCount; i++)
//         {
//             if (tilemap->chunks[i].data)
//                 MemFree(tilemap->chunks[i].data);
//         }
//         MemFree(tilemap->chunks);
//     }

//     tilemap->chunkCount = chunkCount;
//     tilemap->chunks = (TileDataChunk *)MemAlloc(sizeof(TileDataChunk) * chunkCount);
//     chunkCount = 0;

//     int i = 0;
//     while (content[i])
//     {
//         while (content[i] && content[i] <= ' ')
//             i++;
//         char chr = content[i];
//         if (chr == '#')
//         {
//             while (content[i] && content[i] != '\n')
//                 i++;
//             continue;
//         }

//         if (chr == '>')
//         {
//             if (!TileDataChunk_parse(&tilemap->chunks[chunkCount++], content, &i))
//             {
//                 return;
//             }
//         }

//         while (content[i] && content[i] != '\n')
//             i++;
//     }

//     for (int i = 0; i < chunkCount; i++)
//     {
//         if (tilemap->chunks[i].layerCount > tilemap->layerCount)
//         {
//             tilemap->layerCount = tilemap->chunks[i].layerCount;
//         }
//     }
// }

// static TileSprite *_tileSpriteCache = NULL;
// static int _tileSpriteCacheSize = 0;
static char *_tileCache = NULL;
static int _tileCacheSize = 0;

// // this is a naive and simple tile drawing implementation
// void Tilemap_draw(Tilemap *tilemap, Vector2 position, Vector2 scale, int width, int height, int offsetX, int offsetY, Color color)
// {
//     if (tilemap->filepath && GetFileModTime(tilemap->filepath) - 1 > tilemap->fileModTime)
//     {
//         tilemap->fileModTime = GetFileModTime(tilemap->filepath);
//         char *content = LoadFileText(tilemap->filepath);
//         if (content)
//         {
//             Tilemap_parse(tilemap, content);
//         }
//         else
//         {
//             TraceLog(LOG_WARNING, "Failed to load tilemap file %s", tilemap->filepath);
//         }
//     }

//     // the tilemap sprites are stored in a single texture
//     // Each tile type is expressed by a group of sprites in the tileset.
//     // The pattern describes the tile itself and its transitions to transparent
//     // borders. The following visual pattern describes the _vertices_ of each tile where
//     // 0 is transparent and 1 is solid
//     // 0 0 0 0 0
//     // 0 1 1 1 0
//     // 0 1 1 1 0
//     // 0 1 0 1 0
//     // 0 1 1 1 0
//     // 0 0 0 0 0
//     // Each 2x2 square is one tile with each corner representing a vertex. The pattern
//     // 0 1
//     // 0 1
//     // represents a tile where the right side is solid and the left side is transparent.
//     // The pattern
//     // 0 1
//     // 1 0
//     // is not present and must be handled by the drawing logic by combining these two tiles
//     // 0 1 | 0 0
//     // 0 0 | 1 0
//     // The tilemap is stored as a 1D array of unsigned chars where each byte represents a tile
//     // type of a _vertice_ of a tile.
//     // The order of drawing tile types is determined by the ordinal value of the tile type.

//     int minX = offsetX;
//     int minY = offsetY;
//     int maxX = offsetX + width - 1;
//     int maxY = offsetY + height - 1;

//     // let's prepare the data needed for rendering the screen portion
//     int cacheSize = width * height * tilemap->layerCount;
//     if (_tileCacheSize < cacheSize)
//     {
//         if (_tileCache != NULL)
//             MemFree(_tileCache);
//         _tileCache = (char *)MemAlloc(cacheSize);
//         _tileCacheSize = cacheSize;
//     }
//     int spriteCacheSize = width * height * 2;
//     if (_tileSpriteCacheSize < spriteCacheSize)
//     {
//         if (_tileSpriteCache != NULL)
//             MemFree(_tileSpriteCache);
//         _tileSpriteCache = (TileSprite *)MemAlloc(sizeof(TileSprite) * spriteCacheSize);
//         _tileSpriteCacheSize = spriteCacheSize;
//     }

//     int tileSpriteCount = 0;

//     for (int i = 0; i < cacheSize; i++)
//         _tileCache[i] = 0;

//     // fill the chache with procedural content first
//     if (tilemap->getTileData != NULL)
//     {
//         tilemap->getTileData(tilemap, minX, minY, maxX, maxY, tilemap->layerCount, (unsigned char *)_tileCache,
//             _tileSpriteCache, spriteCacheSize, &tileSpriteCount);
//     }

//     // copy the tilemap data to the cache
//     for (int y = minY; y <= maxY; y++)
//     {
//         for (int x = minX; x <= maxX; x++)
//         {
//             for (int i = 0; i < tilemap->chunkCount; i++)
//             {
//                 TileDataChunk *chunk = &tilemap->chunks[i];
//                 if (x < chunk->x || x >= chunk->x + chunk->width || y < chunk->y || y >= chunk->y + chunk->height)
//                     continue;

//                 int cx = x - chunk->x;
//                 int cy = y - chunk->y;
//                 for (int layer = 0; layer < chunk->layerCount; layer++)
//                 {
//                     int index = (cy * chunk->width + cx) * chunk->layerCount + layer;
//                     int cacheIndex = (y - minY) * width * tilemap->layerCount + (x - minX) * tilemap->layerCount + layer;
//                     int value = chunk->data[index];
//                     if (value > 0)
//                         _tileCache[cacheIndex] = value;
//                 }
//             }
//         }
//     }

//     int layerCount = tilemap->layerCount;

//     for (int y = minY; y < maxY; y++)
//     {
//         int tileIndex = (y-minY) * width * layerCount;
//         Vector2 tilePosition = (Vector2){
//             0,
//             position.y + y * tilemap->tileHeight * scale.y};
//         for (int x = minX; x < maxX; x++)
//         {
//             tilePosition.x = position.x + x * tilemap->tileWidth * scale.x;

//             Rectangle destination = {
//                 tilePosition.x, tilePosition.y,
//                 tilemap->tileWidth * scale.x, tilemap->tileHeight * scale.y};

//             for (int layer = 0; layer < layerCount; layer++, tileIndex += 1)
//             {
//                 int typeA = _tileCache[tileIndex] - 1;
//                 int typeB = _tileCache[tileIndex + layerCount] - 1;
//                 int typeC = _tileCache[tileIndex + width * layerCount] - 1;
//                 int typeD = _tileCache[tileIndex + width * layerCount + layerCount] - 1;
//                 int minType = (typeA < typeB) ? typeA : typeB;
//                 minType = (minType < typeC) ? minType : typeC;
//                 minType = (minType < typeD) ? minType : typeD;
//                 int maxType = (typeA > typeB) ? typeA : typeB;
//                 maxType = (maxType > typeC) ? maxType : typeC;
//                 maxType = (maxType > typeD) ? maxType : typeD;
//                 if (minType < 0)
//                 {
//                     if (maxType == -1)
//                         continue;
//                     minType = 0;
//                 }
//                 // iterate over all possible types to determine which ones to draw
//                 for (int tileTypeId = minType; tileTypeId <= maxType; tileTypeId++)
//                 {
//                     unsigned char typeAId = (typeA == tileTypeId) ? 1 : 0;
//                     unsigned char typeBId = (typeB == tileTypeId) ? 1 : 0;
//                     unsigned char typeCId = (typeC == tileTypeId) ? 1 : 0;
//                     unsigned char typeDId = (typeD == tileTypeId) ? 1 : 0;

//                     int tileTypePattern = typeAId << 3 | typeBId << 2 | typeCId << 1 | typeDId << 0;
//                     if (tileTypePattern == 0)
//                         continue;
//                     int hasDiagonalPattern = 0;
//                     // diagonal pattern handling
//                     if (tileTypePattern == 0b1001)
//                     {
//                         hasDiagonalPattern = 0b0001;
//                         tileTypePattern = 0b1000;
//                     }
//                     if (tileTypePattern == 0b0110)
//                     {
//                         hasDiagonalPattern = 0b0010;
//                         tileTypePattern = 0b0100;
//                     }
//                 paintTile:;
//                     TileTypeSpriteLocation spriteLocation = tileTypeSpriteLocations[tileTypePattern];
//                     int srcX = spriteLocation.x + (spriteLocation.width > 0 ? GetLocationRandom(x, y, tileTypeId, spriteLocation.width) : 0);
//                     int srcY = spriteLocation.y + (spriteLocation.height > 0 ? GetLocationRandom(x, y, tileTypeId, spriteLocation.height) : 0);
//                     Rectangle source = (Rectangle){
//                         (tileTypeId * 4 + srcX) * tilemap->tileWidth,
//                         srcY * tilemap->tileHeight,
//                         tilemap->tileWidth, tilemap->tileHeight};
//                     DrawTexturePro(resources.tileset, source, destination,
//                                    (Vector2){0, 0}, 0, color);

//                     if (hasDiagonalPattern)
//                     {
//                         tileTypePattern = hasDiagonalPattern;
//                         hasDiagonalPattern = 0;
//                         goto paintTile;
//                     }
//                 }
//             }
//         }
//     }
// }

void DrawArrow(Camera3D camera, Vector3 a, Vector3 b, float thickness, Color color)
{
    Matrix m = GetCameraMatrix(camera);
    Vector3 points[9];
    Vector3 dir = Vector3Normalize(Vector3Subtract(b, a));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(dir, (Vector3){m.m8, m.m9, m.m10}));
    Vector3 side = Vector3Scale(right, thickness * 0.5f);
    Vector3 c = Vector3Subtract(b, Vector3Scale(dir, thickness * 4.0f));
    Vector3 arrowSide = Vector3Scale(right, thickness * 2.0f);
    points[0] = Vector3Subtract(a, side);
    points[1] = Vector3Add(a, side);
    points[2] = Vector3Subtract(c, side);
    points[3] = Vector3Add(c, side);
    points[4] = points[3];
    points[5] = Vector3Add(c, arrowSide);
    points[6] = Vector3Subtract(c, arrowSide);
    points[7] = b;

    DrawTriangleStrip3D(points, 8, color);
}

int TileMapComponent_getTileType(TileMapComponent *TileMapComponent, int tileX, int tileY, int layer)
{
    if (tileX < 0 || tileX >= TileMapComponent->width || tileY < 0 || tileY >= TileMapComponent->height)
        return 0;
    tileY = TileMapComponent->height - tileY - 1;
    for (int i = 0; i < TileMapComponent->chunkCount; i++)
    {
        TileDataChunk *chunk = &TileMapComponent->chunks[i];
        if (tileX >= chunk->x && tileX < chunk->x + chunk->width && tileY >= chunk->y && tileY < chunk->y + chunk->height)
        {
            int cx = tileX - chunk->x;
            int cy = tileY - chunk->y;
            return chunk->data[(cy * chunk->width + cx) * chunk->layerCount + layer];
        }
    }
    return 0;
}

int TileMapComponent_getTileTypeAtWorldPosition(SceneGraph *graph, SceneObjectId sceneObjectId, SceneComponentId sceneComponent, TileMapComponent *TileMapComponent, Vector3 position, int layer)
{
    SceneObject *sceneObject = SceneGraph_getObject(graph, sceneObjectId);
    if (sceneObject == NULL)
        return 0;
    
    Matrix inv = SceneObject_getToLocalMatrix(sceneObject);
    Vector3 localPosition = Vector3Transform(position, inv);
    int tileX = (int)(localPosition.x / TileMapComponent->tileWidth);
    int tileY = (int)(localPosition.y / TileMapComponent->tileHeight);
    return TileMapComponent_getTileType(TileMapComponent, tileX, tileY, layer);
}

static void TileMapComponent_draw(Camera3D camera, SceneObject *sceneObject, SceneComponentId sceneComponent,
                                  void *componentData, void *userdata)
{
    TileMapComponent *tileMapComponent = (TileMapComponent *)componentData;
    Matrix m = SceneObject_getWorldMatrix(sceneObject);
    // Matrix camM = GetCameraMatrix(camera);

    rlSetTexture(tileMapComponent->tileset.id);

    rlBegin(RL_QUADS);
    rlColor4ub(255,255,255,255);

    float tw = tileMapComponent->tileset.width;
    float th = tileMapComponent->tileset.height;

    int width = tileMapComponent->width;
    int height = tileMapComponent->height;

        // let's prepare the data needed for rendering the screen portion
    int cacheSize = width * height * tileMapComponent->layerCount;
    if (_tileCacheSize < cacheSize)
    {
        if (_tileCache != NULL)
            MemFree(_tileCache);
        _tileCache = (char *)MemAlloc(cacheSize);
        _tileCacheSize = cacheSize;
    }

    for (int i = 0; i < cacheSize; i++)
        _tileCache[i] = 0;

//     // fill the chache with procedural content first
//     if (tilemap->getTileData != NULL)
//     {
//         tilemap->getTileData(tilemap, minX, minY, maxX, maxY, tilemap->layerCount, (unsigned char *)_tileCache,
//             _tileSpriteCache, spriteCacheSize, &tileSpriteCount);
//     }


    int layerCount = tileMapComponent->layerCount;
    int minY = 0;
    int minX = 0;
    int maxX = width;
    int maxY = height;

    // copy the tilemap data to the cache
    // would probably be faster to iterate over each chunk and making the intersection per chunk once
    // instead of for every cell
    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            for (int i = 0; i < tileMapComponent->chunkCount; i++)
            {
                TileDataChunk *chunk = &tileMapComponent->chunks[i];
                if (x < chunk->x || x >= chunk->x + chunk->width || y < chunk->y || y >= chunk->y + chunk->height)
                    continue;

                int cx = x - chunk->x;
                int cy = y - chunk->y;
                for (int layer = 0; layer < chunk->layerCount; layer++)
                {
                    int index = (cy * chunk->width + cx) * chunk->layerCount + layer;
                    int cacheIndex = (y - minY) * width * tileMapComponent->layerCount + (x - minX) * tileMapComponent->layerCount + layer;
                    int value = chunk->data[index];
                    if (value > 0)
                    {
                        _tileCache[cacheIndex] = value;
                    }
                }
            }
        }
    }


    for (int y = minY; y < maxY - 1; y++)
    {
        int tileIndex = (y-minY) * width * layerCount;
        for (int x = minX; x < maxX - 1; x++)
        {
            for (int layer = 0; layer < layerCount; layer++, tileIndex += 1)
            {
                int typeA = _tileCache[tileIndex] - 1;
                int typeB = _tileCache[tileIndex + layerCount] - 1;
                int typeC = _tileCache[tileIndex + width * layerCount] - 1;
                int typeD = _tileCache[tileIndex + width * layerCount + layerCount] - 1;
                int minType = (typeA < typeB) ? typeA : typeB;
                minType = (minType < typeC) ? minType : typeC;
                minType = (minType < typeD) ? minType : typeD;
                int maxType = (typeA > typeB) ? typeA : typeB;
                maxType = (maxType > typeC) ? maxType : typeC;
                maxType = (maxType > typeD) ? maxType : typeD;
                if (minType < 0)
                {
                    if (maxType == -1)
                        continue;
                    minType = 0;
                }
                // iterate over all possible types to determine which ones to draw
                for (int tileTypeId = minType; tileTypeId <= maxType; tileTypeId++)
                {
                    unsigned char typeAId = (typeA == tileTypeId) ? 1 : 0;
                    unsigned char typeBId = (typeB == tileTypeId) ? 1 : 0;
                    unsigned char typeCId = (typeC == tileTypeId) ? 1 : 0;
                    unsigned char typeDId = (typeD == tileTypeId) ? 1 : 0;

                    int tileTypePattern = typeAId << 3 | typeBId << 2 | typeCId << 1 | typeDId << 0;
                    if (tileTypePattern == 0)
                        continue;
                    int hasDiagonalPattern = 0;
                    // diagonal pattern handling
                    if (tileTypePattern == 0b1001)
                    {
                        hasDiagonalPattern = 0b0001;
                        tileTypePattern = 0b1000;
                    }
                    if (tileTypePattern == 0b0110)
                    {
                        hasDiagonalPattern = 0b0010;
                        tileTypePattern = 0b0100;
                    }
                paintTile:;
                    TileTypeSpriteLocation spriteLocation = tileTypeSpriteLocations[tileTypePattern];
                    int srcX = spriteLocation.x + (spriteLocation.width > 0 ? GetLocationRandom(x, y, tileTypeId, spriteLocation.width) : 0);
                    int srcY = spriteLocation.y + (spriteLocation.height > 0 ? GetLocationRandom(x, y, tileTypeId, spriteLocation.height) : 0);
                    Rectangle source = (Rectangle){
                        (tileTypeId * 4 + srcX) * tileMapComponent->tileWidth,
                        srcY * tileMapComponent->tileHeight,
                        tileMapComponent->tileWidth, tileMapComponent->tileHeight};
                    
                    Vector3 pos = (Vector3){m.m12 + x, m.m13 - y, m.m14};
                    
                    rlTexCoord2f((source.x + source.width) / tw, source.y / th);
                    // rlTexCoord2f(1.0f, 0.0f);
                    rlVertex3f(pos.x + 1, pos.y, pos.z);
                    rlTexCoord2f(source.x / tw, source.y / th);
                    // rlTexCoord2f(0.0f, 0.0f);
                    rlVertex3f(pos.x, pos.y, pos.z);
                    

                    rlTexCoord2f(source.x / tw, (source.y + source.height) / th);
                    // rlTexCoord2f(0.0f, 1.0f);
                    rlVertex3f(pos.x, pos.y - 1, pos.z);
                    rlTexCoord2f((source.x + source.width) / tw, (source.y + source.height) / th);
                    // rlTexCoord2f(1.0f, 1.0f);
                    rlVertex3f(pos.x + 1, pos.y - 1, pos.z);

                    if (hasDiagonalPattern)
                    {
                        tileTypePattern = hasDiagonalPattern;
                        hasDiagonalPattern = 0;
                        goto paintTile;
                    }
                }
            }
        }
    }

    rlEnd();

    rlSetTexture(0);
    // DrawBillboardPro(camera, tileMapComponent->tileset, (Rectangle){0,0,32,32}, 
    //     (Vector3){m.m12, m.m13, m.m14}, (Vector3){0.0f,0.0f,1.0f}, (Vector2) {1.0f, 1.0f},
    //     (Vector2){0.0f,0.0f}, 0.0f, WHITE);
    
    DrawArrow(camera, (Vector3){m.m12, m.m13, m.m14}, (Vector3){m.m12 + 10, m.m13, m.m14}, 0.125f, RED);
    DrawArrow(camera, (Vector3){m.m12, m.m13, m.m14}, (Vector3){m.m12, m.m13 - 10, m.m14}, 0.125f, GREEN);
    // DrawLine3D((Vector3){m.m12, m.m13, m.m14}, (Vector3){m.m12, m.m13 + 10, m.m14}, RED);
    // DrawLine3D((Vector3){m.m12, m.m13, m.m14}, (Vector3){m.m12, m.m13, m.m14 + 10}, BLUE);
}

BEGIN_COMPONENT_REGISTRATION(TileMapComponent){
    .draw = TileMapComponent_draw,
} END_COMPONENT_REGISTRATION