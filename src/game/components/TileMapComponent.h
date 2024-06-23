#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(TileMapComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "TileMapComponent.c"

#else

int TileMapComponent_getTileType(TileMapComponent *TileMapComponent, int tileX, int tileY, int layer);
int TileMapComponent_getTileTypeAtWorldPosition(SceneGraph *graph, SceneObjectId sceneObjectId, SceneComponentId sceneComponent, TileMapComponent *TileMapComponent, Vector3 position, int layer);

#endif