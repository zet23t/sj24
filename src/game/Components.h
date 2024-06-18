#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__
#ifndef SERIALIZABLE_STRUCTS_DECLARED
#include "SerializableStructs.h"
#endif

#include "raylib.h"
#include "rlgl.h"
#include "g.h"
#include "shared/scene_graph/scene_graph.h"
#include "shared/resource_manager.h"

AnimationId AnimationManager_getAnimation(AnimationManager *manager, char *filename);
AnimationManager* AnimationManager_getInstance(SceneGraph *sceneGraph);
Animation* AnimationManager_getAnimationById(AnimationManager *manager, AnimationId id);

#endif