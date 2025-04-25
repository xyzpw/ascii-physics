#pragma once

#include "structs/world.h"
#include "constants/control_keys.h"

void scaleActiveControl(World&, double valueStep);
void selectObjectNextOrPrev(World&, CONTROL_KEY key);
