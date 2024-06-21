#include "shared/serialization/define_serialization_macros.h"

#include "components/TileMapComponent.h"
#include "components/mesh_renderer_component.h"
#include "components/playmap_component.h"
#include "components/primitive_renderer_component.h"
#include "components/sprite_renderer_component.h"
#ifdef COMPONENT
COMPONENT(AnimationManager)
COMPONENT(AnimatorComponent)
COMPONENT(SeagullBehaviorComponent)
#endif
#include "components/camera_component.h"
#include "components/AgentBehaviorComponent.h"
#include "SerializableStructs.h"