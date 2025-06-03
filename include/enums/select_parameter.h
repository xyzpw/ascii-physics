#pragma once

#include <string>

enum class SELECT_PARAMETER {
    LAUNCH_ANGLE,
    LAUNCH_VELOCITY,
    OBJECT_MASS,
    WIND_SPEED,
};

inline std::string selectParamToStr(SELECT_PARAMETER param)
{
    switch (param){
        case SELECT_PARAMETER::LAUNCH_ANGLE:
            return "launch angle";
        case SELECT_PARAMETER::LAUNCH_VELOCITY:
            return "launch velocity";
        case SELECT_PARAMETER::OBJECT_MASS:
            return "object mass";
        case SELECT_PARAMETER::WIND_SPEED:
            return "wind velocity";
    }
    return "";
}
