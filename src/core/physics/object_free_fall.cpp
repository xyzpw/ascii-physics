#include <cmath>
#include "core/physics/object_free_fall.h"
#include "core/physics/object_physics.h"
#include "utils/sleep_utils.h"
#include "utils/object_utils.h"
#include "utils/physics_utils.h"
#include "utils/epoch_utils.h"
#include "utils/screen_utils.h"
#include "structs/vector2d.h"

void handleObjectFreeFall(Object& object)
{
    Vector2D& velocityVec = object.vectors.velocity;
    Vector2D& positionVec = object.vectors.position;
    Vector2D& accelerationVec = object.vectors.acceleration;

    const double& COR = object.coefficientOfRestitution;

    bool isFalling = true;
    double epochDeltaStart = getEpochAsDecimal();

    while (true)
    {
        if (!object.isActionUsed){
            break;
        }

        double tDelta = getEpochAsDecimal() - epochDeltaStart;
        epochDeltaStart = getEpochAsDecimal();

        updateObjectPhysicsYAxis(object, tDelta);

        if (positionVec.y <= 0.0){
            positionVec.y = 0.0;
            velocityVec.y = -velocityVec.y * COR;
        }

        if (std::abs(velocityVec.y) < 0.01 &&
            positionVec.y < 0.01 && tDelta > 0)
        {
            positionVec.y = 0.0;
            velocityVec.y = 0.0;
            break;
        }

        sleepCurrentThread(10);
    }
}
