#include <cmath>
#include "core/physics/object_physics.h"
#include "structs/object.h"
#include "utils/physics_utils.h"

void updateObjectPhysicsYAxis(Object& object, double t)
{
    const double& termVel = object.terminalVelocity;
    const double& dragCoefficient = object.dragCoefficient;
    const double& sectionalArea = object.crossSectionalArea;

    Vector2D& velocity = object.vectors.velocity;
    Vector2D& position = object.vectors.position;

    const double& acceleration = object.vectors.acceleration.y;

    const double yScale = velocity.y >= 0.0 ? 1.0 : -1.0;


    newPos += position.y;

    velocity.y = newVelocity;
    position.y = newPos;
}
