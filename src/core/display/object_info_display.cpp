#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include "display/object_info_display.h"
#include "structs/object.h"

void printObjectInfo(Object& object)
{
    Vector2D& positionVector = object.positionVector;
    Vector2D& velocityVector = object.velocityVector;

    std::stringstream infoStr;

    infoStr << std::fixed << std::setprecision(2);
    infoStr << "height: " << positionVector.y << " m";

    if (object.isActionUsed){
        infoStr << "\n";
        infoStr << "velocity: " << velocityVector.getMagnitude() << " m/s";
    }

    mvprintw(0, 0, infoStr.str().c_str());
}
