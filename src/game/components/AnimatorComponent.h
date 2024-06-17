#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(AnimatorComponent)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(AnimatorVariable)
    SERIALIZABLE_CSTR(name)
    SERIALIZABLE_FIELD(float, value)
SERIALIZABLE_STRUCT_END(AnimatorVariable)

SERIALIZABLE_STRUCT_START(AnimatorComponent)
    SERIALIZABLE_CSTR(animationName)
    SERIALIZABLE_FIELD(AnimationId, animationId)
    SERIALIZABLE_FIELD(int, loopCount)
    SERIALIZABLE_FIELD(int, currentStateIndex)
    SERIALIZABLE_FIELD(float, currentTime)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimatorVariable, variables)
SERIALIZABLE_STRUCT_END(AnimatorComponent)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "AnimatorComponent.c"


#else


#endif