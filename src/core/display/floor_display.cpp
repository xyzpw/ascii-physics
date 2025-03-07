#include <ncurses.h>
#include "display/floor_display.h"
#include "constants/object_char_constants.h"
#include "utils/screen_utils.h"

// Print floor at bottom of screen.
void printFloor()
{
    auto windowSize = getWindowSize();

    int length = windowSize.ws_col;
    int floorRow = windowSize.ws_row - 1;

    for (int i = 0; i < length; ++i){
        mvprintw(floorRow, i, FLOOR_CHAR);
    }
}
