#pragma once

#include "enums/object_type.h"
#include "structs/position.h"
#include "structs/vector2d.h"

struct Object {
    OBJECT_TYPE type;
    Position position;

    struct __VectorsList {
        Vector2D velocity{0, 0};
        Vector2D acceleration{0, 0};
        Vector2D position{0, 0};
    };
    __VectorsList vectors;

    double mass;
    double terminalVelocity;
    double dragCoefficient;
    double coefficientOfRestitution = 0.0;
    double crossSectionalArea;

    bool isActionUsed = false;

    Object(OBJECT_TYPE, double kg);
    void reset();
    void simulate();
};
