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

    bool isActionUsed = false;

    Object(OBJECT_TYPE, double kg);
    void drop();
    void reset();
};
