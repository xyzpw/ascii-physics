#pragma once

#include <vector>
#include <string>
#include "structs/object.h"

struct World {
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

    double metersPerChar = 1.0;

    World();
    void setWorldBounds();
    Object& getObjectById(int id);
    void resetObjectById(int id);
};
