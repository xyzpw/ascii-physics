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
