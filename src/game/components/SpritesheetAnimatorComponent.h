#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(SpritesheetAnimatorComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(SpritesheetAnimatorComponent)
    SERIALIZABLE_CSTR(animationName)
    SERIALIZABLE_FIELD(SpritesheetAnimationId, animationId)
    SERIALIZABLE_FIELD(int, loopCount)
    SERIALIZABLE_FIELD(float, currentTime)
SERIALIZABLE_STRUCT_END(SpritesheetAnimatorComponent)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "SpritesheetAnimatorComponent.c"


#else


#endif