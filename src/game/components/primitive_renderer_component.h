#ifdef COMPONENT
// === COMPONENT UTILITIES ===
COMPONENT(PrimitiveRendererComponent)
#elif defined(COMPONENT_DECLARATION)

// === DECLARATIONS ===
#ifndef PRIMITIVE_TYPE_CUBE
#define PRIMITIVE_TYPE_ORIGIN 0
#define PRIMITIVE_TYPE_CUBE 1
#define PRIMITIVE_TYPE_SPHERE 2
#define PRIMITIVE_TYPE_CYLINDER 3
#endif

SERIALIZABLE_STRUCT_START(PrimitiveRendererComponent)
    SERIALIZABLE_FIELD(Color, color)
    SERIALIZABLE_FIELD(Vector3, size)
    SERIALIZABLE_FIELD(int, primitiveType)
    SERIALIZABLE_FIELD(bool, isWireframe)
SERIALIZABLE_STRUCT_END(PrimitiveRendererComponent)

#else

// === DEFINITIONS ===
#include "primitive_renderer_component.c"

#endif