#ifndef __G_H__
#define __G_H__

#include "mem-util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>
#include "game/Components.h"
#include "shared/resource_manager.h"
#include "shared/touch_util.h"
#include "shared/ui/dusk-gui.h"
#include "shared/scene_graph/scene_graph.h"
#include "shared/editor/editor.h"

typedef struct ComponentIdMap {
#define COMPONENT(T) SceneComponentTypeId T##Id;
#include "component_list.h"
#undef COMPONENT
} ComponentIdMap;

extern ComponentIdMap _componentIdMap;

#define BEGIN_COMPONENT_REGISTRATION(T) void T##_register(SceneGraph *g) { _componentIdMap.T##Id = \
    SceneGraph_registerComponentType(g, #T, sizeof(T), (SceneComponentTypeMethods)
#define END_COMPONENT_REGISTRATION );}

extern EditorState* _editorState;
extern ResourceManager* _resourceManager;
extern SceneGraph* _scene;
extern Texture2D _spriteSheet;
extern Model _model;

#endif