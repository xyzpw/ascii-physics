#include <thread>
#include "structs/object.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "constants/physics_constants.h"

const double PI = 3.141592653589793238;

void configureObjectProperties(Object&);

Object::Object(OBJECT_TYPE type, double kg)
{
    this->type = type;
    this->mass = kg;

    configureObjectProperties(*this);

    this->terminalVelocity = calculateTerminalVelocity(*this);
    this->vectors.acceleration = Vector2D{0.0, -GRAVITY_ACCELERATION};
}

void Object::reset()
{
    this->position = getCenterPosition();
    this->vectors.position = positionToVector(this->position);
    this->vectors.velocity = Vector2D{0.0, 0.0};
    this->vectors.acceleration = Vector2D{0.0, -GRAVITY_ACCELERATION};
    this->isActionUsed = false;
    this->terminalVelocity = calculateTerminalVelocity(*this);
    configureObjectProperties(*this);
}

void Object::drop()
{
    std::thread fallThread(handleObjectFreeFall, std::ref(*this));
    fallThread.detach();
}

void configureObjectProperties(Object& object)
{
    switch (object.type)
    {
        case OBJECT_TYPE::OBJECT_BALL:{
            double diameter = 1.0;

            object.dragCoefficient = DRAG_COEFFICIENT_BALL;
            object.crossSectionalArea = (diameter/2.0) * (diameter/2.0) * PI;
        }
    }
}
