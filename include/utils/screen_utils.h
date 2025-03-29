#pragma once

#include <sys/ioctl.h>
#include "structs/position.h"
#include "structs/vector2d.h"

winsize getWindowSize();
bool checkPositionInsideDisplay(Position);
Position getCenterPosition();
Vector2D positionToVector(Position);
Position vectorToPosition(Vector2D);
