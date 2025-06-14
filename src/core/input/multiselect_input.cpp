#include <stdexcept>
#include <algorithm>
#include "core/input/multiselect_input.h"
#include "structs/world.h"
#include "structs/obstacle.h"
#include "structs/vector2d.h"
#include "utils/screen_utils.h"

bool checkMultiselectCanMove(World&, int dCol, int dRow);

void resizeSelectedObstacleY(World& world, int rCount)
{
    if (rCount == 0) return;
    else if (world.entityMultiselect.selectedIds.size() == 0) return;

    bool isRemoving = rCount < 0;

    // Convert unit count to absolute value.
    if (isRemoving) rCount = rCount - rCount * 2.0;

    // Adjust row of object position to fit the position of next object.
    auto fixPosIfIncreasing = [&](Position& top, Position& bottom) {
        if (isRemoving) return;
        top.row -= 1;
        bottom.row += 1;
    };

    // Remove obstacle if resize count is negative, otherwise add.
    auto addOrRemoveObs = [&](Position& p){
        if (isRemoving) world.removeEntityByPosition(p);
        else world.addObstacle(p);
    };

    auto& idList = world.entityMultiselect.selectedIds;

    if (isRemoving && idList.size() <= 1) return;
    else if (!isRemoving &&
            (!checkMultiselectCanMove(world, 0, 1) ||
            !checkMultiselectCanMove(world, 0, -1)))
    {
        return;
    }

    const int& obsCol = world.getActiveEntityPosition().column;
    int obsTopRow = -1, obsBottomRow = -1;

    // Get row number of highest and lowest obstacle.
    for (int i = 0; i < idList.size(); ++i){
        Position& pos = world.getEntityPositionById(idList[i]);

        // Set top/bottom row numbers if they are not already set.
        if (obsTopRow == -1) obsTopRow = pos.row;
        if (obsBottomRow == -1) obsBottomRow = pos.row;

        if (pos.row < obsTopRow) obsTopRow = pos.row;
        if (pos.row > obsBottomRow) obsBottomRow = pos.row;
    }

    for (int i = 0; i < rCount; ++i){
        Position topPos{obsCol, obsTopRow};
        Position bottomPos{obsCol, obsBottomRow};

        fixPosIfIncreasing(topPos, bottomPos);

        addOrRemoveObs(topPos);
        addOrRemoveObs(bottomPos);

        obsTopRow += isRemoving ? 1 : -1;
        obsBottomRow += isRemoving ? -1 : 1;
    }
}

void moveSelectedEntities(World& world, int dCol, int dRow)
{
    if (world.entityMultiselect.selectedIds.size() == 0 ||
        !checkMultiselectCanMove(world, dCol, dRow))
    {
        return;
    }

    auto& idList = world.entityMultiselect.selectedIds;

    for (int i = 0; i < idList.size(); ++i) {
        Position& pos = world.getEntityPositionById(idList[i]);
        Vector2D& vec = world.getEntityVectorPositionById(idList[i]);

        pos.column += dCol;
        pos.row += dRow;
        vec = positionToVector(pos, world.metersPerChar);
    }
}

bool checkMultiselectCanMove(World& world, int dCol, int dRow)
{
    auto& idList = world.entityMultiselect.selectedIds;

    for (const auto& id : idList){
        Position pos = world.getEntityPositionById(id);
        pos.column += dCol;
        pos.row += dRow;
        if (!checkPositionInsideDisplay(pos))
            return false;
    }

    return true;
}
