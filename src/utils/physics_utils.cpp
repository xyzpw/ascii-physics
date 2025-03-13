#include <cmath>
#include "utils/physics_utils.h"
#include "constants/physics_constants.h"
#include "structs/object.h"
#include "enums/object_type.h"

const double g = GRAVITY_ACCELERATION;

// Return distance traveled at a specified time since an object was dropped.
double calculateFreeFallDistanceAtTime(Object& object, double seconds)
{
    const double termVel = object.terminalVelocity;
    bool hasReachedTermVel = g * seconds > termVel;
    if (hasReachedTermVel){
        double timeAtTermVel = termVel / g;
        double distAtTermVel = 0.5 * g * timeAtTermVel * timeAtTermVel;
        double timeSinceTermVel = seconds - timeAtTermVel;
        double distance = timeSinceTermVel * termVel + distAtTermVel;
        return distance;
    }
    return 0.5 * g * seconds * seconds;
}

// Return velocity of an object in free fall at a specified time.
double calculateFreeFallVelocityAtTime(Object& object, double seconds)
{
    const double termVel = object.terminalVelocity;
    double velocity = g * seconds;
    if (velocity > termVel){
        velocity = termVel;
    }
    return velocity;
}

double calculateTerminalVelocity(Object& obj)
{
    OBJECT_TYPE objType = obj.type;
    const double mass = obj.mass;

    const double diameter = 1.0;//NOTE: each char is 1 meter
    switch (objType){
        case OBJECT_TYPE::OBJECT_BALL:{
            double Cd = 0.47;//drag coefficient
            double area = (diameter/2.0) * (diameter/2.0) * M_PI;
            double numerator = 2 * mass * g;
            double denominator = AIR_DENSITY * area * Cd;

            return std::sqrt(numerator / denominator);
            break;
        }
    }
    return 0.0;
}

// Calculate the velocity of an object thrown upward at a specified time.
double calculateVelocityUpAtTime(double velocity, double seconds)
{
    double currentVelocity = velocity - g * seconds;
    return currentVelocity;
}

// Calculate height of object thrown upward at specified time.
double calculateDistanceUpAtTime(double velocity, double seconds)
{
    double currentVelocity = velocity * seconds;
    currentVelocity -= 0.5 * g * seconds * seconds;
    return currentVelocity;
}
