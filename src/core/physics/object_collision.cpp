#include "core/physics/object_collision.h"
#include "structs/object.h"
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

void resolveObjectCollision(Object& objA, Object& objB)
{
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
