#include <thread>
#include "structs/object.h"
#include "utils/screen_utils.h"
#include "utils/object_utils.h"
#include "utils/physics_utils.h"
#include "core/physics/object_free_fall.h"
#include "constants/physics_constants.h"

Object::Object(OBJECT_TYPE type, double kg)
{
    this->type = type;
    this->mass = kg;

    if (type == OBJECT_TYPE::OBJECT_BALL){
        this->dragCoefficient = DRAG_COEFFICIENT_BALL;
    }

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
    this->terminalVelocity = calculateTerminalVelocity(*this);
}
