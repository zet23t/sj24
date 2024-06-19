#ifndef SERIALIZABLE_STRUCT_START
#include "shared/serialization/struct_macros.h"
#endif

// AnimationManager

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
    SERIALIZABLE_FIXED_ARRAY(char, operation, 3)
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

// AnimatorComponent

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

// SeagullBehaviorComponent

SERIALIZABLE_STRUCT_START(SeagullBehaviorComponent)
    SERIALIZABLE_FIELD(float, flyHeight)
SERIALIZABLE_STRUCT_END(SeagullBehaviorComponent)

// AgentBehaviorComponent

SERIALIZABLE_STRUCT_START(AgentBehaviorComponent)
    SERIALIZABLE_FIELD(int, tileWidth)
    SERIALIZABLE_FIELD(int, tileHeight)
SERIALIZABLE_STRUCT_END(AgentBehaviorComponent)
