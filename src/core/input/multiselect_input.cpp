#include <stdexcept>
#include <algorithm>
#include "core/input/multiselect_input.h"
#include "structs/world.h"
#include "structs/obstacle.h"
#include "structs/vector2d.h"
#include "utils/screen_utils.h"

bool checkMultiselectCanMove(World&, int dCol, int dRow);

void resizeSelectedObstacleY(World& world, bool isAdding)
{
    auto& idList = world.entityMultiselect.selectedIds;

    // Remove obstacle if resize count is negative, otherwise add.
    auto addOrRemoveObs = [&](Position& p){
        if (isAdding) world.addObstacle(p);
        else world.removeEntityByPosition(p);
    };

    int obsTopRow = -1, obsBottomRow = -1;
    int leftmostCol = -1, rightmostCol = -1;

    // Get row number of highest and lowest obstacle.
    for (int i = 0; i < idList.size(); ++i){
        Position& pos = world.getEntityPositionById(idList[i]);

        // Set top/bottom row numbers if they are not already set.
        if (obsTopRow == -1) obsTopRow = pos.row;
        if (obsBottomRow == -1) obsBottomRow = pos.row;
        if (leftmostCol == -1) leftmostCol = pos.column;
        if (rightmostCol == -1) rightmostCol = pos.column;

        if (pos.row < obsTopRow) obsTopRow = pos.row;
        if (pos.row > obsBottomRow) obsBottomRow = pos.row;
        if (pos.column < leftmostCol) leftmostCol = pos.column;
        if (pos.column > rightmostCol) rightmostCol = pos.column;
    }

    if (!isAdding && obsTopRow == obsBottomRow) return;

    for (int c = leftmostCol; c < rightmostCol + 1; ++c){
        Position topPos{c, obsTopRow};
        Position bottomPos{c, obsBottomRow};
        if (isAdding) { topPos.row -= 1; bottomPos.row += 1; }
        addOrRemoveObs(topPos);
        addOrRemoveObs(bottomPos);
    }
}

void resizeSelectedObstacleX(World& world, bool isAdding)
{
    auto& idList = world.entityMultiselect.selectedIds;

    int leftmostCol = -1, rightmostCol = -1;
    int topRow = -1, bottomRow = -1;

    // Set value to a specified default if it equals -1.
    auto setIfNeg1 = [&](int& val1, int& val2, const int& def){
        if (val1 == -1) val1 = def;
        if (val2 == -1) val2 = def;
    };

    // Move position to next empty space where the new obstacle will be added.
    auto fixAddingPos = [&](Position& left, Position& right){
        left.column -= 1;
        right.column += 1;
    };

    // Add or remove an obstacle at this position depending on the resize method.
    auto resizeStep = [&](Position& p){
        if (isAdding) world.addObstacle(p);
        else world.removeEntityByPosition(p);
    };

    for (const auto& id : idList){
        auto& pos = world.getEntityPositionById(id);
        int& col = pos.column;
        int& row = pos.row;

        setIfNeg1(leftmostCol, rightmostCol, pos.column);
        setIfNeg1(topRow, bottomRow, pos.row);

        if (col < leftmostCol) leftmostCol = col;
        if (col > rightmostCol) rightmostCol = col;
        if (row < topRow) topRow = row;
        if (row > bottomRow) bottomRow = row;
    }

    if (!isAdding && rightmostCol == leftmostCol) return;

    for (int r = topRow; r < bottomRow + 1; ++r){
        Position posR{rightmostCol, r};
        Position posL{leftmostCol, r};
        if (isAdding) fixAddingPos(posL, posR);
        resizeStep(posR); resizeStep(posL);
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
