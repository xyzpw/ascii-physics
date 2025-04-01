#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include "display/object_info_display.h"
#include "structs/object.h"

void printObjectInfo(Object& object)
{
    Vector2D& positionVector = object.vectors.position;
    Vector2D& velocityVector = object.vectors.velocity;

    std::stringstream infoStr;
    int row = 0;

    infoStr << std::fixed << std::setprecision(2);
    infoStr << "height: " << positionVector.y << " m";

    mvprintw(row++, 0, infoStr.str().c_str());
    infoStr.str("");

    if (object.isActionUsed){
        infoStr << "speed: " << velocityVector.getMagnitude() << " m/s";
        mvprintw(row++, 0, infoStr.str().c_str());
        infoStr.str("");
    }
}
