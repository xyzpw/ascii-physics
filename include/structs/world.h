#pragma once

#include <vector>
#include <string>
#include "structs/object.h"
#include "enums/select_parameter.h"
#include "enums/object_type.h"
#include "constants/physics_constants.h"

// Default values for added objects.
struct _WorldDefaultObjectValues {
    OBJECT_TYPE objectType = OBJECT_TYPE::OBJECT_BALL;
    double objectMass = 1.0;
    double coefficientOfRestitution = 0.0;
    double crossSectionalArea = 1.0;
    double dragCoefficient = DRAG_COEFFICIENT_BALL;
};

struct _ObjectInputInfo {
    double objectLaunchVelocity = 0.0;
    double objectLaunchAngle = 0.0;
    SELECT_PARAMETER selectParameter;
};

struct World {
    bool isSimulating = false;

    std::vector<Object> objects;
    int activeObjectId;

    struct _WorldBorders {
        double ceiling, floor;
        double rightWall, leftWall;
    };
    // Additional info that appears on screen.
    struct _OverlayText {
        std::string text = "";
        bool shouldDisplay = false;
        double displayUntil = 0;
    };
    // Keep track of what stats to display.
    struct _DisplayedStats {
        bool isCollisionCountVisible;
        bool isMaxSpeedVisible;
    };
    _WorldDefaultObjectValues defaultObjectValues;
    _ObjectInputInfo objectInputInfo;

    double metersPerChar = 1.0;

    World();
    void startSimulation();
    void stopSimulation();
    void resetSimulation();
    void setWorldBounds();
    Object& getObjectById(int id);
    void resetObjectById(int id);
    void highlightObjectById(int id, double duration);
    void addObject(OBJECT_TYPE, double kg);
    void removeAllObjects();
};
