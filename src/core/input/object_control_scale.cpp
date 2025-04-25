#include <ncurses.h>
#include <thread>
#include "core/input/object_control_scale.h"
#include "structs/world.h"
#include "structs/object.h"
#include "enums/select_parameter.h"
#include "constants/control_keys.h"

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
    else if (selectParam == SELECT_PARAMETER::OBJECT_MASS){
        minValue = 0;
        double& targetValue = object.mass;
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

void selectObjectNextOrPrev(World& world, CONTROL_KEY key)
{
    int currentIndex = -1;

    for (int i = 0; i < world.objects.size(); ++i)
    {
        if (world.objects[i].id == world.activeObjectId){
            currentIndex = i;
            break;
        }
    }

    if (currentIndex == -1)
        return;

    bool canIncrement = world.objects.size() > currentIndex + 1;

    if (CONTROL_KEY::KEY_OBJECT_SELECT_NEXT == key && canIncrement){
        world.activeObjectId = world.objects[currentIndex + 1].id;
    }
    else if (CONTROL_KEY::KEY_OBJECT_SELECT_PREV == key && 0 < currentIndex){
        world.activeObjectId = world.objects[currentIndex - 1].id;
    }
}
