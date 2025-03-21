#pragma once

#include <sys/ioctl.h>
#include "structs/position.h"
#include "structs/vector2d.h"

winsize getWindowSize();
Position getCenterPosition();
Vector2D positionToVector(Position);
