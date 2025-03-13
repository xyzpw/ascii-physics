#include "core/physics/object_free_fall.h"
#include "utils/sleep_utils.h"
#include "utils/object_utils.h"
#include "utils/physics_utils.h"
#include "utils/epoch_utils.h"

void handleObjectFreeFall(Object& object)
{
    double& height = object.height;
    double& velocity = object.velocity;
    double& objectMass = object.mass;

    const double epochAtStart = getEpochAsDecimal();
    const double initHeight = height;
    const double termVelocity = object.terminalVelocity;
    while (true)
    {
        if (!object.isActionUsed) {
            break;
        }

        double airtime = getEpochAsDecimal() - epochAtStart;
        double distanceTraveled = calculateFreeFallDistanceAtTime(object, airtime);
        double currentVelocity = calculateFreeFallVelocityAtTime(object, airtime);

        height = initHeight - distanceTraveled;
        if (height < 0.0){
            height = 0.0;
        }
        velocity = currentVelocity;

        int rowAtHeight = getRowAtHeight(height);
        object.position.row = rowAtHeight;

        if (height <= 0.0){
            break;
        }

        sleepCurrentThread(20);
    }

    handleObjectFallBounce(object, velocity);

    velocity = 0.0;
}

void handleObjectFallBounce(Object& object, double velocity)
{
    double& objectHeight = object.height;
    double& objectVelocity = object.velocity;
    int& objectPosRow = object.position.row;

    const double COR = object.coefficientOfRestitution;
    velocity = COR * velocity;

    // Set variables necessary for bounce loop.
    bool isFalling = false;
    double initHeight = objectHeight;
    double epochAirtime = getEpochAsDecimal();

    // Bounce loop.
    while (true)
    {
        if (!object.isActionUsed){
            break;
        }

        double elapsedAirtime = getEpochAsDecimal() - epochAirtime;

        double newVelocity;
        double newUpDistance;
        if (isFalling)
        {
            newVelocity = calculateFreeFallVelocityAtTime(object, elapsedAirtime);
            double fallDistance = calculateFreeFallDistanceAtTime(
                object, elapsedAirtime
            );

            objectHeight = initHeight - fallDistance;
            objectVelocity = newVelocity;

            if (objectHeight <= 0.0){
                objectHeight = 0.0;
                objectVelocity = 0.0;
                velocity = COR * velocity;
                isFalling = false;
            }
            else {
                objectHeight = initHeight - fallDistance;
                objectVelocity = newVelocity;
            }
        }
        else { //object is bouncing back up
            newVelocity = calculateVelocityUpAtTime(velocity, elapsedAirtime);
            newUpDistance = calculateDistanceUpAtTime(velocity, elapsedAirtime);

            if (newVelocity <= 0.0){
                isFalling = true;
                epochAirtime = getEpochAsDecimal();
                initHeight = objectHeight;
            }
            else {
                objectHeight = newUpDistance;
                objectVelocity = newVelocity;
            }
        }

        objectPosRow = getRowAtHeight(objectHeight);

        // There is no reason to continue if the velocity and height are this low.
        if (velocity <= 0.01){
            objectHeight = 0.0;
            objectVelocity = 0.0;
            break;
        }

        sleepCurrentThread(20);
    }
}
