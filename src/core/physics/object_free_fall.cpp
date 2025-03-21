#include "core/physics/object_free_fall.h"
#include "utils/sleep_utils.h"
#include "utils/object_utils.h"
#include "utils/physics_utils.h"
#include "utils/epoch_utils.h"
#include "utils/screen_utils.h"
#include "structs/vector2d.h"

void handleObjectFreeFall(Object& object)
{
    Vector2D& objectVelocityVector = object.velocityVector;
    Vector2D& objectPositionVector = object.positionVector;

    const double epochAtStart = getEpochAsDecimal();

    const double initPosY = objectPositionVector.y;
    while (true)
    {
        if (!object.isActionUsed){
            break;
        }

        double airtime = getEpochAsDecimal() - epochAtStart;

        double velocity = calculateFreeFallVelocityAtTime(
            object, airtime
        );
        double distance = calculateFreeFallDistanceAtTime(
            object, airtime
        );

        objectVelocityVector.y = -velocity;
        objectPositionVector.y = initPosY - distance;

        if (objectPositionVector.y <= 0.0){
            objectPositionVector.y = 0.0;
        }

        object.position.row = getRowAtHeight(objectPositionVector.y);

        if (objectPositionVector.y == 0.0){
            objectVelocityVector.y = -objectVelocityVector.y;
            break;
        }

        sleepCurrentThread(10);
    }

    handleObjectFallBounce(object, objectVelocityVector.y);

    objectVelocityVector = Vector2D{0.0, 0.0};
}

void handleObjectFallBounce(Object& object, double velocity)
{
    Vector2D& objectVelocityVector = object.velocityVector;
    Vector2D& objectPositionVector = object.positionVector;

    const double COR = object.coefficientOfRestitution;

    velocity *= COR; //multiply upward velocity by coefficient of restitution

    bool isFalling = false;

    double initPosY = object.positionVector.y;

    double epochAirtime = getEpochAsDecimal();
    while (true)
    {
        if (!object.isActionUsed){
            break;
        }

        double elapsedAirtime = getEpochAsDecimal() - epochAirtime;

        if (isFalling)
        {
            double velocityFreeFall = calculateFreeFallVelocityAtTime(
                object, elapsedAirtime
            );
            double distanceFreeFall = calculateFreeFallDistanceAtTime(
                object, elapsedAirtime
            );

            objectVelocityVector.y = -velocityFreeFall;
            objectPositionVector.y = initPosY - distanceFreeFall;

            if (object.positionVector.y <= 0.0){
                isFalling = false;

                objectPositionVector.y = 0.0;

                velocity *= COR;

                objectVelocityVector.y = velocity;
            }
        }
        else //object is bouncing back up
        {
            double velocityAtTime = calculateVelocityUpAtTime(
                velocity, elapsedAirtime
            );
            double distanceAtTime = calculateDistanceUpAtTime(
                velocity, elapsedAirtime
            );

            if (velocityAtTime <= 0.0){
                isFalling = true;
                epochAirtime = getEpochAsDecimal();
                initPosY = objectPositionVector.y;
            }
            else {
                objectVelocityVector.y = velocityAtTime;
                objectPositionVector.y = distanceAtTime;
            }
        }

        object.position.row = getRowAtHeight(objectPositionVector.y);

        // There is no reason to continue if the velocity and height are this low.
        if (velocity <= 0.01){
            object.velocityVector.y = 0.0;
            object.positionVector.y = 0.0;
            break;
        }

        sleepCurrentThread(10);
    }
}
