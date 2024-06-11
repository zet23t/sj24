#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(SeagullBehaviorComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(SeagullBehaviorComponent)
    SERIALIZABLE_FIELD(int, tileWidth)
    SERIALIZABLE_FIELD(int, tileHeight)
SERIALIZABLE_STRUCT_END(SeagullBehaviorComponent)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "SeagullBehaviorComponent.c"


#else


#endif