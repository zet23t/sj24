#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(TileMapComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===
SERIALIZABLE_STRUCT_START(TileDataChunk)
    SERIALIZABLE_FIELD(int, x)
    SERIALIZABLE_FIELD(int, y)
    SERIALIZABLE_FIELD(int, width)
    SERIALIZABLE_FIELD(int, height)
    SERIALIZABLE_FIELD(int, layerCount)
    NONSERIALIZED_FIELD(unsigned char*, data)
SERIALIZABLE_STRUCT_END(TileDataChunk)

SERIALIZABLE_STRUCT_START(TileMapComponent)
    SERIALIZABLE_FIELD(int, tileWidth)
    SERIALIZABLE_FIELD(int, tileHeight)
    SERIALIZABLE_FIELD(int, width)
    SERIALIZABLE_FIELD(int, height)
    SERIALIZABLE_FIELD(int, layerCount)
    NONSERIALIZED_FIELD(TileDataChunk*, chunks)
    NONSERIALIZED_FIELD(int, chunkCount)
    NONSERIALIZED_FIELD(Texture2D, tileset)
    NONSERIALIZED_FIELD(Mesh, mesh)
    NONSERIALIZED_FIELD(Material, material)
SERIALIZABLE_STRUCT_END(TileMapComponent)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "TileMapComponent.c"
int TileMapComponent_getTileType(TileMapComponent *TileMapComponent, int tileX, int tileY, int layer);


#else


#endif