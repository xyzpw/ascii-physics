#include <ncurses.h>
#include "core/ncurses_loop.h"
#include "setup/ncurses_setup.h"
#include "core/input/keyboard_input.h"
#include "utils/screen_utils.h"
#include "display/floor_display.h"
#include "utils/object_utils.h"
#include "display/object_info_display.h"
#include "constants/object_char_constants.h"

void startNcursesLoop(Object object)
{
    initializeNcurses();

    // Reference variables used to access object while keeping code clean.
    Position &objectPosition = object.position;
    int& objectColumn = object.position.column;
    int& objectRow = object.position.row;
    double& currentObjectHeight = object.height;

    while (true)
    {
        clear();

        char keyPressed = getch();

        if (keyPressed == 'q'){
            object.reset();
            break;
        }

        handleKeyPress(keyPressed, object);

        mvprintw(objectRow, objectColumn, OBJECT_BALL_CHAR);

        if (!object.isActionUsed){
            currentObjectHeight = getObjectHeight(object);
        }

        printObjectInfo(object);
        printFloor();

        refresh();
        napms(20);
    }

    endwin();
}
