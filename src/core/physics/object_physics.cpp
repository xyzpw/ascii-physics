#include <cmath>
#include "core/physics/object_physics.h"
#include "core/physics/object_collision.h"
#include "structs/object.h"
#include "utils/physics_utils.h"

const double VELOCITY_Y_MIN = 0.01;

void updateObjectPhysicsYAxis(Object& object, double t)
{
    Vector2D& velocity = object.vectors.velocity;
    Vector2D& position = object.vectors.position;

    bool hasCollidedFloor = checkHasCollidedWithFloor(object);
    if (hasCollidedFloor){
        position.y = 0.0;
        velocity.y *= -object.coefficientOfRestitution;
        if (std::abs(velocity.y) < VELOCITY_Y_MIN){
            velocity.y = 0.0;
            return;
        }
    }

    double dragForce = calculateDragForce(
        velocity.y, object.dragCoefficient, object.crossSectionalArea
    );

    const double& acceleration = object.vectors.acceleration.y;

    const double yScale = velocity.y >= 0.0 ? 1.0 : -1.0;

    double netAcceleration = acceleration - (dragForce / object.mass) * yScale;

    double newVelocity = calculateVelocityFinal(velocity.y, netAcceleration, t);

    double newPos = calculateDisplacement(newVelocity, netAcceleration, t);

    // Set velocity and position to 0 if object can't bounce.
    if (hasCollidedFloor && newPos <= 0.0){
        velocity.y = 0.0;
        position.y = 0.0;
        return;
    }

    newPos += position.y;

    velocity.y = newVelocity;
    position.y = newPos;
}
