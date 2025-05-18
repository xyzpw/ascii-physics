#include <ncurses.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "display/info_display.h"
#include "structs/world.h"
#include "structs/position.h"
#include "structs/vector2d.h"
#include "utils/screen_utils.h"
#include "utils/epoch_utils.h"

void displayWorldMiscDisplayText(World&);
void displayObjectInfo(Object&);
void displayWorldObjectStats(World&);
void displayButtons(World&);

void displaySimulationText(World& world)
{
    bool& shouldDisplay = world.overlayText.shouldDisplay;
    double& displayUntil = world.overlayText.displayUntil;

    const double epoch = getEpochAsDecimal();

    if (shouldDisplay && epoch < displayUntil){
        displayWorldMiscDisplayText(world);
    }
    else if (shouldDisplay){
        shouldDisplay = false;
    }

    Object& activeObject = world.getObjectById(world.activeObjectId);

    displayObjectInfo(activeObject);
    displayWorldObjectStats(world);
    if (world.menuPanel.isOpen)
        displayButtons(world);
}

void displayWorldMiscDisplayText(World& world)
{
    std::string text = world.overlayText.text;

    Position center = getCenterPosition();
    center.column -= text.length() / 2;

    mvprintw(center.row, center.column, text.c_str());
}

void displayObjectInfo(Object& object)
{
    Vector2D& position = object.vectors.position;
    Vector2D& velocity = object.vectors.velocity;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2);

    int row = 0;

    stream << "height: " << position.y << " m";
    mvprintw(row++, 0, stream.str().c_str());
    stream.str("");

    stream << "speed: " << velocity.getMagnitude() << " m/s";
    mvprintw(row++, 0, stream.str().c_str());
    stream.str("");
}

void displayWorldObjectStats(World& world)
{
    Object& object = world.getObjectById(world.activeObjectId);
    auto& visibleStats = world.displayedStats;
    auto& stats = object.statistics;

    const Position centerPos = getCenterPosition();
    const int centerCol = centerPos.column;
    int row = 0;

    std::stringstream stream;

    auto printStat = [&](bool canDisplay, std::string label, auto val,
                         std::string suffix="")
    {
        if (canDisplay){
            stream << label << val << suffix;
            int col = centerCol - stream.str().length() / 2;
            mvprintw(row++, col, stream.str().c_str());
            stream.str("");
        }
    };

    printStat(visibleStats.isCollisionCountVisible, "collisions: ", stats.collisionCount);
    printStat(visibleStats.isMaxSpeedVisible, "max speed: ", stats.maxSpeed, " m/s");
    printStat(visibleStats.isMaxHeightVisible, "max height: ", stats.maxHeight, " m");
}

void displayButtons(World& world)
{
    const int& col = world.menuPanel.colMin;
    for (const auto& it : world.menuPanel.items){
        mvprintw(it.second.row, col, it.second.text.c_str());
    }
}
