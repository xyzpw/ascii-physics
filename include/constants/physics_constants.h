#pragma once

inline double GRAVITY_ACCELERATION = 9.80665;
inline const double AIR_DENSITY = 1.225;

inline const double DRAG_COEFFICIENT_BALL = 0.47;

inline void changeGravityAcceleration(double v){
    GRAVITY_ACCELERATION = v;
}
