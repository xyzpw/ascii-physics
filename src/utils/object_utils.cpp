#include <cmath>
#include "utils/object_utils.h"
#include "utils/screen_utils.h"

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
