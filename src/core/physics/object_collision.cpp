#include "core/physics/object_collision.h"
#include "structs/object.h"
#include "structs/vector2d.h"

bool checkHasCollidedWithFloor(Object& object)
{
    return object.vectors.position.y <= 0.0;
}
