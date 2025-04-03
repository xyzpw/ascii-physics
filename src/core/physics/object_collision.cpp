#include "core/physics/object_collision.h"

bool checkHasCollidedWithFloor(Object& object)
{
    return object.vectors.position.y <= 0.0;
}
