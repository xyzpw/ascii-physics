#pragma once

#include <vector>
#include "enums/object_type.h"
#include "structs/position.h"
#include "structs/vector2d.h"
#include "constants/object_char_constants.h"

struct Trail {
    Position position;
    int frameAge = 0;
    int colorPairNum = PAIR_NUM_BRIGHT_GRAY_FG;
    bool isOld = false;
    const char* displayChar = ":";
};

struct Object {
    OBJECT_TYPE type;
    Position position;

    struct _VectorsList {
        Vector2D velocity{0, 0};
        Vector2D acceleration{0, 0};
        Vector2D position{0, 0};
    } vectors;

    struct _Stats {
        int collisionCount = 0;
        double maxSpeed = 0;
        double maxHeight = 0;
    } statistics;

    struct _HighlightInfo {
        bool isHighlighted = false;
        bool isHitFlashing = false;
        double highlightUntil;
        double hitFlashingUntil;
    } highlightInfo;

    double mass;
    double terminalVelocity;
    double dragCoefficient;
    double coefficientOfRestitution = 0.0;
    double crossSectionalArea;
    double mLength; //length of object

    std::vector<Trail> trails; // trails to highlight (if enabled)

    bool isActionUsed = false;
    bool isFrozen = false;

    int id;

    Object(OBJECT_TYPE, double kg);
    void resetVectors();
    void resetPositionToCenter(double charSize);
    void resetActionStates();
    void resetPhysicsProperties(double charSize);
    void addTrail(Position pos);
    void updateTrails();
    void removeExpiredTrails();
    void launch(Vector2D velocity);
    void slingshot(Vector2D restPos);
};
