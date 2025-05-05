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
    bool isTakingInput = false;
    double takeInputUntil;
    std::string customInputValue = "";
};

struct World {
    bool isSimulating = false;

    std::vector<Object> objects;
    int activeObjectId;

    struct _WorldBorders {
        double ceiling, floor;
        double rightWall, leftWall;
    } worldBounds;

    // Additional info that appears on screen.
    struct _OverlayText {
        std::string text = "";
        bool shouldDisplay = false;
        double displayUntil = 0;
    } overlayText;

    // Keep track of what stats to display.
    struct _DisplayedStats {
        bool isCollisionCountVisible;
        bool isMaxSpeedVisible;
        bool isMaxHeightVisible = false;
    } displayedStats;

    struct _VisualEffects {
        bool isHitFlashesVisible;
        bool isTrailVisible = false;
    } visualEffects;

    _WorldDefaultObjectValues defaultObjectValues;
    _ObjectInputInfo objectInputInfo;

    double metersPerChar = 1.0;

    World();
    void startSimulation();
    void stopSimulation();
    void resetSimulation();
    void setWorldBounds();
    void setOverlayText(std::string text, double duration=1.0);
    Object& getObjectById(int id);
    void resetObjectById(int id);
    void highlightObjectById(int id, double duration);
    void hitFlashObjectById(int id, double duration);
    void addObject(OBJECT_TYPE, double kg);
    void removeAllObjects();
};
