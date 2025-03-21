#include <cmath>
#include "structs/vector2d.h"

double Vector2D::getMagnitude()
{
    double x2 = x * x;
    double y2 = y * y;

    return std::sqrt(x2 + y2);
}
