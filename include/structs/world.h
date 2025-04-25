#pragma once

#include <vector>
#include <string>
#include "structs/object.h"
#include "enums/select_parameter.h"
#include "enums/object_type.h"
#include "constants/physics_constants.h"

// Default values for added objects.
struct __WorldDefaultObjectValues {
    OBJECT_TYPE objectType = OBJECT_TYPE::OBJECT_BALL;
    double objectMass = 1.0;
    double coefficientOfRestitution = 0.0;
    double crossSectionalArea = 1.0;
    double dragCoefficient = DRAG_COEFFICIENT_BALL;
};

struct __ObjectInputInfo {
    double objectLaunchVelocity = 0.0;
    double objectLaunchAngle = 0.0;
    SELECT_PARAMETER selectParameter;
};

struct World {
    bool isSimulating = false;

    std::vector<Object> objects;
    int activeObjectId;

    struct __WorldBorders {
        double ceiling, floor;
        double rightWall, leftWall;
    };
    // Additional info that appears on screen.
    struct __OverlayText {
        std::string text = "";
        bool shouldDisplay = false;
        double displayUntil = 0;
    };
    __WorldBorders worldBounds;
    __OverlayText overlayText;
    __WorldDefaultObjectValues defaultObjectValues;
    __ObjectInputInfo objectInputInfo;

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
