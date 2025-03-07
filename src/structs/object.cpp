#include <thread>
#include "structs/object.h"
#include "utils/screen_utils.h"
#include "utils/object_utils.h"
#include "utils/physics_utils.h"
#include "core/physics/object_free_fall.h"

Object::Object(OBJECT_TYPE type, double kg)
{
    this->type = type;
    this->mass = kg;
    this->terminalVelocity = calculateTerminalVelocity(*this);
}

void Object::drop()
{
    std::thread fallThread(handleObjectFreeFall, std::ref(*this));
    fallThread.detach();
}

void Object::reset()
{
    this->position = getCenterPosition();
    this->height = getObjectHeight(*this);
    this->velocity = 0.0;
    this->isActionUsed = false;
}
