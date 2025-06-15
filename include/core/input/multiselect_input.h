#pragma once

#include "structs/world.h"

/*
    Add/remove 2 obstacles above and below current selected ones.

    @param isAdding: true if increasing size, false if decreasing.
*/
void resizeSelectedObstacleY(World& world, bool isAdding);

/*
    Resize selected obstacles horizontally for both sides.

    @param isAdding: true if increasing size, false if decreasing
*/
void resizeSelectedObstacleX(World& world, bool isAdding);

// Move selected entities.
void moveSelectedEntities(World& world, int dCol, int dRow);
