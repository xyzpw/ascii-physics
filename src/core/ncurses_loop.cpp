#include <ncurses.h>
#include "structs/world.h"
#include "core/ncurses_loop.h"
#include "setup/ncurses_setup.h"
#include "core/input/keyboard_input.h"
#include "utils/screen_utils.h"
#include "display/floor_display.h"
#include "display/info_display.h"
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
            world.resetObjectById(object.id);
            isRunning = false;
        }

        handleKeyPress(keyPressed, world);

        object.position = vectorToPosition(object.vectors.position / world.metersPerChar);

        if (checkPositionInsideDisplay(objectPosition)){
            mvprintw(objectRow, objectColumn, OBJECT_BALL_CHAR);
        }

        displaySimulationText(world);
        printFloor();

        refresh();
        napms(20);
    }

    endwin();
}
