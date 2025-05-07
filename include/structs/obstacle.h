#pragma once

#include "structs/position.h"
#include "structs/vector2d.h"

struct Obstacle {
    Position position;

    double mLength;

    int id;

    struct _VectorList {
        Vector2D position{0, 0};
    } vectors;
};
