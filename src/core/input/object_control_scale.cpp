#include <ncurses.h>
#include <thread>
#include "core/input/object_control_scale.h"
#include "structs/world.h"
#include "structs/object.h"
#include "enums/select_parameter.h"

void scaleValue(double& value, double step, double min, double max);

void scaleActiveControl(World& world, double valueStep)
{
    Object& object = world.getObjectById(world.activeObjectId);

    SELECT_PARAMETER selectParam = world.objectInputInfo.selectParameter;

    double minValue = 0;
    double maxValue = 2'147'483'648;

    if (selectParam == SELECT_PARAMETER::LAUNCH_ANGLE){
        minValue = 0;
        maxValue = 360;
        double& targetValue = world.objectInputInfo.objectLaunchAngle;
        scaleValue(targetValue, valueStep, minValue, maxValue);
    }
    else if (selectParam == SELECT_PARAMETER::LAUNCH_VELOCITY){
        double& targetValue = world.objectInputInfo.objectLaunchVelocity;
        scaleValue(targetValue, valueStep, minValue, maxValue);
    }
}

void scaleValue(double& value, double step, double min, double max)
{
    double isIncreasing = 0 < step;

    if ((isIncreasing && max >= value + step) ||
        (!isIncreasing && min <= value + step))
    {
        value += step;
    }
    else {
        value = isIncreasing ? max : min;
    }
}
