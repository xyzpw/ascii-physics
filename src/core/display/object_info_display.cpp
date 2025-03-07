#include <ncurses.h>
#include <sstream>
#include <iomanip>
#include "display/object_info_display.h"
#include "structs/object.h"

void printObjectInfo(Object& object)
{
    double& mass = object.mass;
    double& height = object.height;
    double& velocity = object.velocity;

    std::stringstream infoStr;

    infoStr << std::fixed << std::setprecision(2);
    infoStr << "height: " << height << " m";

    if (object.isActionUsed){
        infoStr << "\n";
        infoStr << "velocity: " << velocity << " m/s";
    }

    mvprintw(0, 0, infoStr.str().c_str());
}
