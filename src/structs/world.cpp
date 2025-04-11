#include <stdexcept>
#include <sstream>
#include "structs/world.h"
#include "structs/object.h"
#include "utils/screen_utils.h"

World::World()
{
    auto windowSize = getWindowSize();

    auto& bounds = this->worldBounds;

    bounds.rightWall = windowSize.ws_col - 1;
    bounds.leftWall = 0;
    bounds.ceiling = (windowSize.ws_row - 1);
    bounds.floor = 0;
}

Object& World::getObjectById(int id)
{
    for (auto& obj : this->objects){
        if (obj.id == id)
            return obj;
    }

    std::stringstream errMsg;
    errMsg << "no object with id " << id << " exists";

    throw std::invalid_argument(errMsg.str());
}
