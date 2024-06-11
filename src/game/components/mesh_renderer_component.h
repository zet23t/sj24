#undef COMPONENT_NAME
#define COMPONENT_NAME MeshRendererComponent

#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(MeshRendererComponent)
#elif defined(COMPONENT_DECLARATION)

// === DECLARATIONS ===

SERIALIZABLE_STRUCT_START(MeshRendererComponent)
    SERIALIZABLE_ANNOTATION(Range, Vector2, (&(Vector2){0.0f, 1.0f}))
    SERIALIZABLE_FIELD(float, litAmount)
    NONSERIALIZED_FIELD(Material*, material)
    NONSERIALIZED_FIELD(Mesh*, mesh)
SERIALIZABLE_STRUCT_END(MeshRendererComponent)

#else

// === DEFINITIONS ===
#include "mesh_renderer_component.c"

#endif

