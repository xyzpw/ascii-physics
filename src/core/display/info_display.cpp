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

    if (object.isActionUsed){
        stream << "speed: " << velocity.getMagnitude() << " m/s";
        mvprintw(row++, 0, stream.str().c_str());
        stream.str("");
    }
}
