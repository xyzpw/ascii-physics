#include <cmath>
#include "core/physics/object_physics.h"
#include "structs/object.h"
#include "utils/physics_utils.h"

void updateObjectPhysicsYAxis(Object& object, double t)
{
    Vector2D& velocity = object.vectors.velocity;
    Vector2D& position = object.vectors.position;

    double dragForce = calculateDragForce(
        velocity.y, object.dragCoefficient, object.crossSectionalArea
    );

    const double& acceleration = object.vectors.acceleration.y;

    const double yScale = velocity.y >= 0.0 ? 1.0 : -1.0;

    double netAcceleration = acceleration - (dragForce / object.mass) * yScale;

    double newVelocity = calculateVelocityFinal(velocity.y, netAcceleration, t);

    double newPos = calculateDisplacement(newVelocity, netAcceleration, t);
    newPos += position.y;

    velocity.y = newVelocity;
    position.y = newPos;
}
