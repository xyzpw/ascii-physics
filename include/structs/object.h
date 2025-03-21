#pragma once

#include "enums/object_type.h"
#include "structs/position.h"
#include "structs/vector2d.h"

struct Object {
    OBJECT_TYPE type;
    Position position;

    Vector2D positionVector{0, 0};
    Vector2D velocityVector{0, 0};

    double mass;
    double height;
    double velocity = 0.0;
    double terminalVelocity;
    double dragCoefficient;
    double coefficientOfRestitution = 0.0;

    bool isActionUsed = false;

    Object(OBJECT_TYPE, double kg);
    void reset();
    void drop();
};
