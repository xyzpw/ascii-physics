#include <cmath>
#include "utils/physics_utils.h"
#include "constants/physics_constants.h"
#include "structs/object.h"
#include "structs/vector2d.h"
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

double calculateDragForce(double velocity, double dragCoeff, double area)
{
    double vSquared = velocity * velocity;

    return 0.5 * AIR_DENSITY * vSquared * dragCoeff * area;
}

double calculateVelocityFinal(double velocity, double acceleration, double t)
{
    return velocity + acceleration * t;
}

double calculateDisplacement(double velocity, double acceleration, double t)
{
    return velocity * t + 0.5 * acceleration * t * t;
}

Vector2D degreesToVector(double deg)
{
    double rad = deg * M_PI / 180.0;

    double x = std::cos(rad);
    double y = std::sin(rad);

    return Vector2D{x, y};
}
