#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(PlayMapComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===
SERIALIZABLE_STRUCT_START(PlayMapComponent)
    SERIALIZABLE_FIELD(int, width)
    SERIALIZABLE_FIELD(int, height)
    NONSERIALIZED_FIELD(Mesh, mesh)
    NONSERIALIZED_FIELD(Material, material)
SERIALIZABLE_STRUCT_END(PlayMapComponent)

SERIALIZABLE_STRUCT_START(TileType)
    SERIALIZABLE_FIELD(uint8_t, rotation)
    SERIALIZABLE_FIELD(uint8_t, type)
SERIALIZABLE_STRUCT_END(TileType)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "playmap_component.c"

#else
#define TILE_TYPE_WATER 0
#define TILE_TYPE_CURRENT 1
#define TILE_TYPE_WHIRL 2

TileType PlayMapComponent_getTileType(PlayMapComponent* playMap, int x, int y);

#endif