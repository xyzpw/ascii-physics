#pragma once

#include "structs/object.h"

bool checkHasCollidedWithFloor(Object&);
bool checkObjectsCollided(Object&, Object&);
void resolveObjectCollision(Object&, Object&);
