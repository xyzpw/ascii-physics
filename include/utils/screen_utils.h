#pragma once

#include <sys/ioctl.h>
#include "structs/position.h"
#include "structs/vector2d.h"

winsize getWindowSize();
bool checkPositionInsideDisplay(Position);
Position getCenterPosition();
Vector2D positionToVector(Position, double charSize=1.0);
Position vectorToPosition(Vector2D, double charSize=1.0);
