#ifndef SERIALIZABLE_STRUCT_START
#include "shared/serialization/struct_macros.h"
#endif

// TileMapComponent

SERIALIZABLE_STRUCT_START(TileDataChunk)
    SERIALIZABLE_FIELD(int, x)
    SERIALIZABLE_FIELD(int, y)
    SERIALIZABLE_FIELD(int, width)
    SERIALIZABLE_FIELD(int, height)
    SERIALIZABLE_FIELD(int, layerCount)
    NONSERIALIZED_FIELD(unsigned char*, data)
SERIALIZABLE_STRUCT_END(TileDataChunk)

SERIALIZABLE_STRUCT_START(TileMapComponent)
    SERIALIZABLE_FIELD(bool, cacheIsUpdated)
    SERIALIZABLE_FIELD(int, tileWidth)
    SERIALIZABLE_FIELD(int, tileHeight)
    SERIALIZABLE_FIELD(int, width)
    SERIALIZABLE_FIELD(int, height)
    SERIALIZABLE_FIELD(int, layerCount)
    NONSERIALIZED_FIELD(float*, cachedTiles)
    NONSERIALIZED_FIELD(int, cachedTileCount)
    NONSERIALIZED_FIELD(int, cachedTileCapacity)
    NONSERIALIZED_FIELD(TileDataChunk*, chunks)
    NONSERIALIZED_FIELD(int, chunkCount)
    NONSERIALIZED_FIELD(Texture2D, tileset)
    NONSERIALIZED_FIELD(Mesh, mesh)
    NONSERIALIZED_FIELD(Material, material)
SERIALIZABLE_STRUCT_END(TileMapComponent)

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
    SERIALIZABLE_FIELD(int8_t, varA)
    SERIALIZABLE_FIELD(int8_t, varB)
    SERIALIZABLE_FIELD(int8_t, operation)
    SERIALIZABLE_FIELD(float, valueA)
    SERIALIZABLE_FIELD(float, valueB)
SERIALIZABLE_STRUCT_END(AnimationCondition)

SERIALIZABLE_STRUCT_START(AnimationStateTransition)
    SERIALIZABLE_FIELD(int, target)
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

// AgentBehaviorComponent

SERIALIZABLE_STRUCT_START(AgentBehaviorComponent)
    SERIALIZABLE_FIELD(int, tileWidth)
    SERIALIZABLE_FIELD(int, tileHeight)
SERIALIZABLE_STRUCT_END(AgentBehaviorComponent)

// SpriteRendererComponent

SERIALIZABLE_STRUCT_START(SpriteAsset)
    NONSERIALIZED_FIELD(Texture2D, texture)
    SERIALIZABLE_FIELD(Rectangle, source)
    SERIALIZABLE_FIELD(Vector4, scale9frame)
SERIALIZABLE_STRUCT_END(SpriteAsset)

SERIALIZABLE_STRUCT_START(SpriteRendererComponent)
    SERIALIZABLE_FIELD(SpriteAsset, spriteAsset)
    SERIALIZABLE_FIELD(int, sortId)
    SERIALIZABLE_FIELD(int, previousSortIndex)
    SERIALIZABLE_FIELD(Color, tint)
    SERIALIZABLE_FIELD(Vector2, pivot)
    SERIALIZABLE_FIELD(float, pixelsPerUnit)
    SERIALIZABLE_FIELD(float, snapping)
    SERIALIZABLE_FIELD(Vector2, size)
    SERIALIZABLE_FIELD(Vector2, flip)
SERIALIZABLE_STRUCT_END(SpriteRendererComponent)

// SeagullBehaviorComponent

SERIALIZABLE_STRUCT_START(SeagullBehaviorComponent)
    SERIALIZABLE_FIELD(SceneComponentId, animator)
    SERIALIZABLE_FIELD(SceneObjectId, sprite)
    SERIALIZABLE_FIELD(SceneComponentId, shadow)
    SERIALIZABLE_FIELD(float, flyHeight)
    SERIALIZABLE_FIELD(float, time)
    SERIALIZABLE_FIELD(float, nextDecisionTime)
    SERIALIZABLE_FIELD(Vector2, moveTarget)
    SERIALIZABLE_FIELD(uint8_t, flyHeightVarIndex)
    SERIALIZABLE_FIELD(uint8_t, onWaterVarIndex)
SERIALIZABLE_STRUCT_END(SeagullBehaviorComponent)

