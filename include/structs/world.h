#pragma once

#include <vector>
#include <string>
#include "structs/object.h"
#include "structs/obstacle.h"
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
    bool isSlingshotMode = false;
};

struct World {
    bool isSimulating = false;

    std::vector<Object> objects;
    std::vector<Obstacle> obstacles;
    std::vector<int> entityIdSpawnOrder;
    int activeObjectId;
    int activeEntityId = -1;

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
        bool isCollisionCountVisible = false;
        bool isMaxSpeedVisible = false;
        bool isMaxHeightVisible = false;
    } displayedStats;

    struct _VisualEffects {
        bool isHitFlashesVisible = false;
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
    bool checkObjectIdExists(int id);
    Obstacle& getObstacleById(int id);
    Vector2D& getActiveEntityVectorPosition();
    Position& getActiveEntityPosition();
    void resetObjectById(int id);
    void highlightObjectById(int id, double duration=0.5);
    void hitFlashObjectById(int id, double duration=0.5);
    void addObject(OBJECT_TYPE, double kg, Position pos={-1,-1});
    void addObstacle();
    void removeAllObjects();
    void removeAllObstacles();
};
