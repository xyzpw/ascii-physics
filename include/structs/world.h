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

    World();
    Object& getObjectById(int id);
};
