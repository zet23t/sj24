#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(AgentBehaviorComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(AgentBehaviorComponent)
    SERIALIZABLE_FIELD(int, tileWidth)
    SERIALIZABLE_FIELD(int, tileHeight)
SERIALIZABLE_STRUCT_END(AgentBehaviorComponent)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "AgentBehaviorComponent.c"


#else


#endif