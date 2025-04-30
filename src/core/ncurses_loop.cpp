#include <ncurses.h>
#include "structs/world.h"
#include "core/ncurses_loop.h"
#include "setup/ncurses_setup.h"
#include "core/input/keyboard_input.h"
#include "utils/screen_utils.h"
#include "display/floor_display.h"
#include "display/info_display.h"
#include "constants/object_char_constants.h"

void updatePosAndDisplayObjects(World&);

void startNcursesLoop(World world)
{
    initializeNcurses();

    Object& object = world.getObjectById(world.activeObjectId);

    bool isRunning = true;
    while (isRunning)
    {
        clear();

        if (!isRunning){
            break;
        }

        char keyPressed = getch();

        if (keyPressed == 'q'){
            world.stopSimulation();
            isRunning = false;
        }

        handleKeyPress(keyPressed, world);

        // Display all object in the simulation.
        updatePosAndDisplayObjects(world);

        displaySimulationText(world);
        printFloor();

        refresh();
        napms(20);
    }

    endwin();
}

void updatePosAndDisplayObjects(World& world)
{
    for (auto& obj : world.objects)
    {
        obj.position = vectorToPosition(obj.vectors.position / world.metersPerChar);
        const Position& pos = obj.position;

        if (!checkPositionInsideDisplay(pos)){
            continue;
        }

        mvprintw(pos.row, pos.column, OBJECT_BALL_CHAR);

        if (obj.highlightInfo.isHighlighted)
        {
            mvchgat(
                pos.row, pos.column, 1, A_NORMAL,
                PAIR_NUM_YELLOW_FG, nullptr
            );
        }
    }
}
