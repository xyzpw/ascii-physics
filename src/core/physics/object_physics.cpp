#include <cmath>
#include "core/physics/object_physics.h"
#include "structs/object.h"
#include "utils/physics_utils.h"

void updateObjectPhysicsYAxis(Object& object, double t)
{
    const double& termVel = object.terminalVelocity;

    Vector2D& velocity = object.vectors.velocity;
    Vector2D& position = object.vectors.position;

    const double& acceleration = object.vectors.acceleration.y;

    const double yScale = velocity.y >= 0.0 ? 1.0 : -1.0;

    double newVelocity = calculateVelocityFinal(velocity.y, acceleration, t);
    newVelocity = std::abs(newVelocity) < termVel ? newVelocity : termVel * yScale;

    double newPos = calculateDisplacement(newVelocity, acceleration, t);
    newPos += position.y;

    velocity.y = newVelocity;
    position.y = newPos;
}
