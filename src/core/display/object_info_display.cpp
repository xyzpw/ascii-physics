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

    infoStr << std::fixed << std::setprecision(2);
    infoStr << "height: " << positionVector.y << " m";

    if (object.isActionUsed){
        infoStr << "speed: " << velocityVector.getMagnitude() << " m/s";
    }
}
