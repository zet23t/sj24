#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(AnimationManager)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(AnimationKey)
    SERIALIZABLE_FIELD(float, time)
    SERIALIZABLE_CSTR(objectPath)
    NONSERIALIZED_FIELD(SceneComponentTypeId, targetTypeId)
    NONSERIALIZED_FIELD(int, targetDataOffset)
    NONSERIALIZED_FIELD(int, dataSize)
    NONSERIALIZED_FIELD(void*, data)
SERIALIZABLE_STRUCT_END(AnimationKey)

SERIALIZABLE_STRUCT_START(AnimationClip)
    SERIALIZABLE_CSTR(name)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationKey, frames)
    SERIALIZABLE_FIELD(int, loopCount)
SERIALIZABLE_STRUCT_END(AnimationClip)

SERIALIZABLE_STRUCT_START(Animation)
    SERIALIZABLE_CSTR(filename)
    NONSERIALIZED_FIELD(long, lastModifiedTime)
    SERIALIZABLE_FIELD(int, generation)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationClip, frames)
SERIALIZABLE_STRUCT_END(Animation)

SERIALIZABLE_STRUCT_START(AnimationId)
    SERIALIZABLE_FIELD(int, animationIndex)
    SERIALIZABLE_FIELD(int, generation)
SERIALIZABLE_STRUCT_END(AnimationId)

SERIALIZABLE_STRUCT_START(AnimationManager)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(Animation, animations)
    SERIALIZABLE_FIELD(int, generationCounter)
SERIALIZABLE_STRUCT_END(AnimationManager)

#elif !defined(DEFINITIONS)

// === DEFINITIONS ===
#include "AnimationManager.c"
AnimationId AnimationManager_getAnimation(AnimationManager *manager, char *filename);
AnimationManager* AnimationManager_getInstance(SceneGraph *sceneGraph);

#else


#endif