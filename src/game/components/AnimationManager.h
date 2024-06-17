#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(AnimationManager)
#elif defined(COMPONENT_DECLARATION)
// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(AnimationKey)
    SERIALIZABLE_FIELD(float, time)
    SERIALIZABLE_FIELD(float, value)
SERIALIZABLE_STRUCT_END(AnimationKey)

SERIALIZABLE_STRUCT_START(AnimationTrack)
    SERIALIZABLE_CSTR(path)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationKey, keys)
SERIALIZABLE_STRUCT_END(AnimationTrack)

SERIALIZABLE_STRUCT_START(AnimationClip)
    SERIALIZABLE_CSTR(name)
    SERIALIZABLE_FIELD(float, duration)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationTrack, tracks)
    SERIALIZABLE_FIELD(int, loopCount)
SERIALIZABLE_STRUCT_END(AnimationClip)

SERIALIZABLE_STRUCT_START(AnimationVariable)
    SERIALIZABLE_CSTR(name)
    SERIALIZABLE_FIELD(float, value)
SERIALIZABLE_STRUCT_END(AnimationVariable)

SERIALIZABLE_STRUCT_START(AnimationCondition)
    SERIALIZABLE_CSTR(varA)
    SERIALIZABLE_CSTR(varB)
    SERIALIZABLE_CSTR(operation)
    SERIALIZABLE_FIELD(float, valueA)
    SERIALIZABLE_FIELD(float, valueB)
SERIALIZABLE_STRUCT_END(AnimationCondition)

SERIALIZABLE_STRUCT_START(AnimationStateTransition)
    SERIALIZABLE_CSTR(target)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationCondition, conditions)
SERIALIZABLE_STRUCT_END(AnimationStateTransition)

SERIALIZABLE_STRUCT_START(AnimationState)
    SERIALIZABLE_CSTR(name)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(uint16_t, clipSequence)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationStateTransition, transitions)
SERIALIZABLE_STRUCT_END(AnimationState)

SERIALIZABLE_STRUCT_START(Animation)
    SERIALIZABLE_CSTR(filename)
    NONSERIALIZED_FIELD(int, lastModifiedTime)
    SERIALIZABLE_FIELD(int, generation)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationClip, clips)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationState, states)
    SERIALIZABLE_STRUCT_LIST_ELEMENT(AnimationVariable, variables)
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

#else

AnimationId AnimationManager_getAnimation(AnimationManager *manager, char *filename);
AnimationManager* AnimationManager_getInstance(SceneGraph *sceneGraph);
Animation* AnimationManager_getAnimationById(AnimationManager *manager, AnimationId id);

#endif