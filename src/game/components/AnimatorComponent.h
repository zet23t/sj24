#ifndef _ANIMATOR_COMPONENT_H_
#define _ANIMATOR_COMPONENT_H_

#include "game/Components.h"

int AnimatorComponent_getVariableIndex(AnimatorComponent *component, const char *name);
void AnimatorComponent_setVariableValue(AnimatorComponent *component, int index, float value);

#endif