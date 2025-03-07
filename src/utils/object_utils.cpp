#include <cmath>
#include "utils/object_utils.h"
#include "utils/screen_utils.h"

double getObjectHeight(Object& object)
{
    auto windowSize = getWindowSize();
    int floorRow = windowSize.ws_row - 1;

    double height = floorRow - object.position.row;
    height -= 1;

    return height;
}

int getRowAtHeight(double height)
{
    auto windowSize = getWindowSize();
    int floorRow = windowSize.ws_row - 1;

    int row = std::round(floorRow - height);
    if (row >= floorRow){
        row = floorRow - 1;
    }

    return row;
}
