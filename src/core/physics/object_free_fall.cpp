#include "core/physics/object_free_fall.h"
#include "utils/sleep_utils.h"
#include "utils/object_utils.h"
#include "utils/physics_utils.h"

void handleObjectFreeFall(Object& object)
{
    double airtime = 0.0;

    double& height = object.height;
    double& velocity = object.velocity;
    double& objectMass = object.mass;

    const double initHeight = height;
    const double termVelocity = object.terminalVelocity;
    while (height > 0.0)
    {
        double distanceTraveled = calculateFreeFallDistanceAtTime(object, airtime);
        double currentVelocity = calculateFreeFallVelocityAtTime(object, airtime);

        height = initHeight - distanceTraveled;
        if (height < 0.0){
            height = 0.0;
        }
        velocity = currentVelocity;

        int rowAtHeight = getRowAtHeight(height);
        object.position.row = rowAtHeight;

        sleepCurrentThread(20);
        airtime += 0.02;
    }
    velocity = 0.0;
}
