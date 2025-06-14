#pragma once

#include "structs/world.h"

/*
    Add/remove 2 obstacles above and below current selected ones.

    @param rCount: units to vertically scale obstacle count on each end
*/
void resizeSelectedObstacleY(World& world, int rCount);

// Move selected entities.
void moveSelectedEntities(World& world, int dCol, int dRow);
