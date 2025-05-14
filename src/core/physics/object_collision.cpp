#include <cmath>
#include "core/physics/object_collision.h"
#include "structs/object.h"
#include "structs/obstacle.h"
#include "structs/world.h"
#include "structs/vector2d.h"

bool checkHasCollidedWithFloor(Object& object)
{
    return object.vectors.position.y <= 0.0;
}

bool checkObjectsCollided(Object& objA, Object& objB)
{
    Vector2D& posA = objA.vectors.position;
    Vector2D& posB = objB.vectors.position;

    if (objA.vectors.velocity.getMagnitude() <= 0 &&
        objB.vectors.velocity.getMagnitude() <= 0)
    {
        return false;
    }

    float dx = posA.x - posB.x;
    float dy = posA.y - posB.y;
    double distSq = dx * dx + dy * dy;
    double minDist = objA.mLength/2.0 + objB.mLength/2.0;

    return distSq <= minDist * minDist;
}

bool checkCircleSquareCollision(Object& circle, Obstacle& square)
{
    if (circle.vectors.velocity.getMagnitude() <= 0)
        return false;

    double circleX = circle.vectors.position.x;
    double circleY = circle.vectors.position.y;
    double radius  = circle.mLength / 2.0;

    double halfSize = square.mLength / 2.0;
    double squareX = square.vectors.position.x;
    double squareY = square.vectors.position.y;

    // Clamp circle center to the nearest point on the square.
    double closestX = std::max(squareX - halfSize, std::min(circleX, squareX + halfSize));
    double closestY = std::max(squareY - halfSize, std::min(circleY, squareY + halfSize));

    // Compute distance to that point.
    double dx = circleX - closestX;
    double dy = circleY - closestY;

    return (dx * dx + dy * dy) <= (radius * radius);
}


void resolveObjectCollision(Object& objA, Object& objB)
{
    if (objA.isFrozen) objA.isFrozen = false;
    if (objB.isFrozen) objB.isFrozen = false;

    Vector2D delta = objA.vectors.position - objB.vectors.position;
    double distance = delta.getMagnitude();
    double minDist = (objA.mLength + objB.mLength) / 2.0;

    if (distance < minDist && distance > 0.0)
    {
        // Normalize the delta to get the collision normal.
        Vector2D normal = delta / distance;

        // Minimum translation distance to push objects apart.
        double overlap = minDist - distance;
        Vector2D correction = normal * (overlap / 2.0);
        objA.vectors.position += correction;
        objB.vectors.position -= correction;

        // Relative velocity.
        Vector2D relVel = objA.vectors.velocity - objB.vectors.velocity;

        // Calculate relative velocity along the normal.
        double velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;

        // Do not resolve if velocities are separating.
        if (velAlongNormal > 0)
            return;

        double e = (objA.coefficientOfRestitution + objB.coefficientOfRestitution) / 2.0;
        double invMassA = 1.0 / objA.mass;
        double invMassB = 1.0 / objB.mass;

        // Calculate impulse scalar.
        double j = -(1 + e) * velAlongNormal;
        j /= (invMassA + invMassB);

        // Apply impulse.
        Vector2D impulse = normal * j;
        objA.vectors.velocity += impulse * invMassA;
        objB.vectors.velocity -= impulse * invMassB;
    }
}

void resolveCircleSquareCollision(Object& circle, Obstacle& square)
{
    const double REST_VELOCITY_THRESHOLD = 0.01;
    const double MIN_SEPARATION = 0.01;

    double radius = circle.mLength / 2.0;
    double halfSize = square.mLength / 2.0;

    double circleX = circle.vectors.position.x;
    double circleY = circle.vectors.position.y;
    double squareX = square.vectors.position.x;
    double squareY = square.vectors.position.y;

    // Find closest point on square to circle
    double closestX = std::max(squareX - halfSize, std::min(circleX, squareX + halfSize));
    double closestY = std::max(squareY - halfSize, std::min(circleY, squareY + halfSize));

    double dx = circleX - closestX;
    double dy = circleY - closestY;
    double distSq = dx * dx + dy * dy;

    if (distSq == 0.0) {
        dx = 1.0;
        dy = 0.0;
        distSq = 1.0;
    }

    double distance = std::sqrt(distSq);
    double overlap = radius - distance;

    if (overlap > 0.0)
    {
        // Normalize direction.
        double normX = dx / distance;
        double normY = dy / distance;

        // Push out by overlap and small extra separation to prevent sinking.
        double separation = overlap + MIN_SEPARATION;
        circle.vectors.position.x += normX * separation;
        circle.vectors.position.y += normY * separation;

        // Reflect velocity.
        double dot = circle.vectors.velocity.x * normX + circle.vectors.velocity.y * normY;

        double vBeforeSq = circle.vectors.velocity.x * circle.vectors.velocity.x +
                           circle.vectors.velocity.y * circle.vectors.velocity.y;

        // Stop bounce if velocity is very low.
        if (std::sqrt(vBeforeSq) < REST_VELOCITY_THRESHOLD) {
            circle.vectors.velocity.x = 0.0;
            circle.vectors.velocity.y = 0.0;
        } else {
            // Reflect.
            circle.vectors.velocity.x -= 2 * dot * normX;
            circle.vectors.velocity.y -= 2 * dot * normY;

            // Apply restitution.
            circle.vectors.velocity.x *= circle.coefficientOfRestitution;
            circle.vectors.velocity.y *= circle.coefficientOfRestitution;
        }
    }
}
