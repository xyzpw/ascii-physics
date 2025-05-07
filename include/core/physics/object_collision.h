#pragma once

#include "structs/object.h"
#include "structs/obstacle.h"

bool checkHasCollidedWithFloor(Object&);
bool checkObjectsCollided(Object&, Object&);
bool checkCircleSquareCollision(Object&, Obstacle&);
void resolveObjectCollision(Object&, Object&);
void resolveCircleSquareCollision(Object&, Obstacle&);
