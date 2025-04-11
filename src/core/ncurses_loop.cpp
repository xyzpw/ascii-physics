#include <ncurses.h>
#include "structs/world.h"
#include "core/ncurses_loop.h"
#include "setup/ncurses_setup.h"
#include "core/input/keyboard_input.h"
#include "utils/screen_utils.h"
#include "display/floor_display.h"
#include "display/object_info_display.h"
#include "constants/object_char_constants.h"

void startNcursesLoop(World world)
{
    initializeNcurses();

    Object& object = world.getObjectById(world.activeObjectId);

    // Reference variables used to access object while keeping code clean.
    Position &objectPosition = object.position;
    int& objectColumn = object.position.column;
    int& objectRow = object.position.row;

    bool isRunning = true;
    while (isRunning)
    {
        clear();

        if (!isRunning){
            break;
        }

        char keyPressed = getch();

        if (keyPressed == 'q'){
            object.reset();
            isRunning = false;
        }

        handleKeyPress(keyPressed, object);

        object.position = vectorToPosition(object.vectors.position);

        if (checkPositionInsideDisplay(objectPosition)){
            mvprintw(objectRow, objectColumn, OBJECT_BALL_CHAR);
        }

        printObjectInfo(object);
        printFloor();

        refresh();
        napms(20);
    }

    endwin();
}
