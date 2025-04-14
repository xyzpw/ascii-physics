#include <thread>
#include "structs/object.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "utils/random_utils.h"
#include "constants/physics_constants.h"
#include "core/physics/object_physics.h"

const double PI = 3.141592653589793238;

void configureObjectProperties(Object&);

Object::Object(OBJECT_TYPE type, double kg)
{
    this->type = type;
    this->mass = kg;

    this->terminalVelocity = calculateTerminalVelocity(*this);
    this->vectors.acceleration = Vector2D{0.0, -GRAVITY_ACCELERATION};

    this->id = randInt();
}

void Object::reset()
{
    this->position = getCenterPosition();
    this->vectors.position = positionToVector(this->position);
    this->vectors.velocity = Vector2D{0.0, 0.0};
    this->vectors.acceleration = Vector2D{0.0, -GRAVITY_ACCELERATION};
    this->isActionUsed = false;
    this->terminalVelocity = calculateTerminalVelocity(*this);
}

void Object::launch(Vector2D velocity)
{
    this->vectors.velocity += velocity;
}
