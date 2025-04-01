#include <cmath>
#include "utils/physics_utils.h"
#include "constants/physics_constants.h"
#include "structs/object.h"
#include "enums/object_type.h"

const double& g = GRAVITY_ACCELERATION;

double calculateTerminalVelocity(Object& obj)
{
    OBJECT_TYPE objType = obj.type;
    const double mass = obj.mass;

    const double Cd = obj.dragCoefficient;
    const double& area = obj.crossSectionalArea;
    switch (objType){
        case OBJECT_TYPE::OBJECT_BALL:{
            double numerator = 2 * mass * g;
            double denominator = AIR_DENSITY * area * Cd;

            return std::sqrt(numerator / denominator);
            break;
        }
    }
    return 0.0;
}

double calculateVelocityFinal(double velocity, double acceleration, double t)
{
    return velocity + acceleration * t;
}

double calculateDisplacement(double velocity, double acceleration, double t)
{
    return velocity * t + 0.5 * acceleration * t * t;
}
