#include <cmath>
#include "core/physics/object_physics.h"
#include "core/physics/object_collision.h"
#include "structs/object.h"
#include "structs/world.h"
#include "utils/physics_utils.h"
#include "utils/sleep_utils.h"

const double VELOCITY_Y_MIN = 0.01;
const double VELOCITY_X_MIN = 0.01;

void updateObjectPhysicsYAxis(World& world, Object& object, double t)
{
    Vector2D& velocity = object.vectors.velocity;
    Vector2D& position = object.vectors.position;

    bool hasCollidedFloor = checkHasCollidedWithFloor(object);
    if (hasCollidedFloor){
        position.y = 0.0;
        if (velocity.y < 0){
            velocity.y *= -object.coefficientOfRestitution;
        }
        if (std::abs(velocity.y) < VELOCITY_Y_MIN){
            velocity.y = 0.0;
            if (velocity.x < VELOCITY_X_MIN)
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
        newVelocity = 0.0;
        newPos = 0.0;
        if (velocity.x < VELOCITY_Y_MIN)
            return;
    }

    newPos += position.y;

    // Bounce off ceiling.
    if (newPos >= world.worldBounds.ceiling - world.metersPerChar){
        newPos = world.worldBounds.ceiling - world.metersPerChar;
        newVelocity *= -object.coefficientOfRestitution;
    }

    velocity.y = newVelocity;
    position.y = newPos;
}

void updateObjectPhysicsXAxis(World& world, Object& object, double tDelta)
{
    Vector2D& velocity = object.vectors.velocity;
    Vector2D& position = object.vectors.position;
    Vector2D& acceleration = object.vectors.acceleration;
    const double& mass = object.mass;

    double dragForce = calculateDragForce(
        velocity.x, object.dragCoefficient, object.crossSectionalArea
    );

    const double xScale = velocity.x >= 0.0 ? 1.0 : -1.0;

    double netAccelerationX = acceleration.x - (dragForce / mass) * xScale;

    double newVelocity = calculateVelocityFinal(velocity.x, netAccelerationX, tDelta);

    double newPos = calculateDisplacement(newVelocity, netAccelerationX, tDelta);
    newPos += position.x;

    // Bounce off of walls.
    if (newPos <= world.worldBounds.leftWall){
        newPos = world.worldBounds.leftWall;
        newVelocity *= -object.coefficientOfRestitution;
    }
    else if (newPos >= world.worldBounds.rightWall){
        newPos = world.worldBounds.rightWall;
        newVelocity *= -object.coefficientOfRestitution;
    }

    velocity.x = newVelocity;
    position.x = newPos;
}

void updateObjectVectors(World& world, Object& object, double tDelta)
{
    updateObjectPhysicsXAxis(world, object, tDelta);
    updateObjectPhysicsYAxis(world, object, tDelta);

    // Check for collision between objects.
    for (int i = 0; i < world.objects.size(); ++i)
    {
        for (int j = i + 1; j < world.objects.size(); ++j)
        {
            Object& A = world.objects[i];
            Object& B = world.objects[j];

            if (checkObjectsCollided(A, B))
            {
                resolveObjectCollision(A, B);
            }
        }
    }
}

void simulateObjectsInWorld(World& world)
{
    const double deltaInterval = 0.01;

    while (world.isSimulating)
    {
        for (auto& object : world.objects){
            updateObjectVectors(world, object, deltaInterval);
        }
        sleepCurrentThread(10);
    }
}
