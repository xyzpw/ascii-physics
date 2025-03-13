#pragma once

#include "enums/object_type.h"
#include "structs/position.h"

struct Object {
    OBJECT_TYPE type;
    Position position;

    double mass;
    double height;
    double velocity = 0.0;
    double terminalVelocity;
    double coefficientOfRestitution = 0.0;

    bool isActionUsed = false;

    Object(OBJECT_TYPE, double kg);
    void drop();
    void reset();
};
