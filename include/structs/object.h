#pragma once

#include "enums/object_type.h"
#include "structs/position.h"
#include "structs/vector2d.h"

struct Object {
    OBJECT_TYPE type;
    Position position;

    struct _VectorsList {
        Vector2D velocity{0, 0};
        Vector2D acceleration{0, 0};
        Vector2D position{0, 0};
    };
    struct _Stats {
        int collisionCount = 0;
    };
    _VectorsList vectors;
    _Stats statistics;

    double mass;
    double terminalVelocity;
    double dragCoefficient;
    double coefficientOfRestitution = 0.0;
    double crossSectionalArea;
    double mLength; //length of object

    bool isActionUsed = false;

    bool isHighlighted = false;

    int id;

    Object(OBJECT_TYPE, double kg);
    void resetVectors();
    void resetPositionToCenter(double charSize);
    void resetActionStates();
    void resetPhysicsProperties(double charSize);
    void launch(Vector2D velocity);
};
