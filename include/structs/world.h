#pragma once

#include <vector>
#include "structs/object.h"

struct World {
    std::vector<Object> objects;
    int activeObjectId;

    struct __WorldBorders {
        double ceiling, floor;
        double rightWall, leftWall;
    };
    __WorldBorders worldBounds;

    double metersPerChar = 1.0;

    World();
    Object& getObjectById(int id);
};
