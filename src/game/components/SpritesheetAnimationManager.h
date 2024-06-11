#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(SpritesheetAnimationManager)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(SpritesheetFrame)
    SERIALIZABLE_FIELD(float, frameStart)
    SERIALIZABLE_FIELD(float, frameEnd)
    SERIALIZABLE_FIELD(Rectangle, source)
SERIALIZABLE_STRUCT_END(SpritesheetFrame)

SERIALIZABLE_STRUCT_START(SpritesheetAnimationClip)
    SERIALIZABLE_CSTR(name)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(SpritesheetFrame, frames)
    SERIALIZABLE_FIELD(int, loopCount)
SERIALIZABLE_STRUCT_END(SpritesheetAnimationClip)

SERIALIZABLE_STRUCT_START(SpritesheetAnimation)
    SERIALIZABLE_CSTR(filename)
    NONSERIALIZED_FIELD(long, lastModifiedTime)
    SERIALIZABLE_FIELD(int, generation)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(SpritesheetAnimationClip, frames)
SERIALIZABLE_STRUCT_END(SpritesheetAnimation)

SERIALIZABLE_STRUCT_START(SpritesheetAnimationId)
    SERIALIZABLE_FIELD(int, animationIndex)
    SERIALIZABLE_FIELD(int, generation)
SERIALIZABLE_STRUCT_END(SpritesheetAnimationId)

SERIALIZABLE_STRUCT_START(SpritesheetAnimationManager)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(SpritesheetAnimation, animations)
    SERIALIZABLE_FIELD(int, generationCounter)
SERIALIZABLE_STRUCT_END(SpritesheetAnimationManager)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "SpritesheetAnimationManager.c"
SpritesheetAnimationId SpritesheetAnimationManager_getAnimation(SpritesheetAnimationManager *manager, char *filename);
SpritesheetAnimationManager* SpritesheetAnimationManager_getInstance(SceneGraph *sceneGraph);

#else


#endif