#include <ncurses.h>
#include "structs/world.h"
#include "core/ncurses_loop.h"
#include "setup/ncurses_setup.h"
#include "core/input/keyboard_input.h"
#include "core/input/mouse_input.h"
#include "utils/screen_utils.h"
#include "utils/epoch_utils.h"
#include "display/floor_display.h"
#include "display/info_display.h"
#include "constants/object_char_constants.h"

void updatePosAndDisplayObjects(World&);
void displayObstacles(World&);
void fixHighlightColors(Object& object);

void startNcursesLoop(World world)
{
    initializeNcurses();

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
        handleMouseClick(keyPressed, world);

        // Display all object in the simulation.
        updatePosAndDisplayObjects(world);
        displayObstacles(world);

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
        obj.updateTrails();
        for (auto& tr : obj.trails){
            const Position& pos = tr.position;
            if (!checkPositionInsideDisplay(pos)) continue;

            mvprintw(pos.row, pos.column, tr.displayChar);
            mvchgat(pos.row, pos.column, 1, A_NORMAL, tr.colorPairNum, nullptr);
        }
        obj.removeExpiredTrails();
    }

    for (auto& obj : world.objects)
    {
        obj.position = vectorToPosition(obj.vectors.position / world.metersPerChar);
        const Position& pos = obj.position;

        if (!checkPositionInsideDisplay(pos)){
            continue;
        }

        mvprintw(pos.row, pos.column, OBJECT_BALL_CHAR);

        if (obj.highlightInfo.isHighlighted || obj.highlightInfo.isHitFlashing)
        {
            const int pairNum = obj.highlightInfo.isHighlighted ?
                                PAIR_NUM_YELLOW_FG :
                                PAIR_NUM_RED_FG;
            mvchgat(pos.row, pos.column, 1, A_NORMAL, pairNum, nullptr);
            fixHighlightColors(obj);
        }
    }
}

void displayObstacles(World& world)
{
    for (const auto& ob : world.obstacles)
    {
        const Position& pos = ob.position;
        if (!checkPositionInsideDisplay(pos)) continue;

        mvprintw(pos.row, pos.column, OBSTACLE_CHAR);
    }
}

// Disable object highlighting if highlight until time has expired.
void fixHighlightColors(Object& object)
{
    auto& info = object.highlightInfo;

    if (info.isHighlighted && getEpochAsDecimal() > info.highlightUntil){
        info.isHighlighted = false;
    }

    if (info.isHitFlashing && getEpochAsDecimal() > info.hitFlashingUntil){
        info.isHitFlashing = false;
    }
}
