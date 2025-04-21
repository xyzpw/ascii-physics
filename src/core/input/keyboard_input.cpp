#include <thread>
#include <sstream>
#include "core/input/keyboard_input.h"
#include "core/input/object_control_scale.h"
#include "core/physics/object_physics.h"
#include "structs/object.h"
#include "structs/world.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "utils/epoch_utils.h"
#include "constants/control_keys.h"

bool checkIsControlKeyMovement(CONTROL_KEY);
void changeSelectParam(World&, SELECT_PARAMETER);
void changeSelectParamValueOnInput(World&, SELECT_PARAMETER, bool isPositive);

void handleKeyPress(const char key, World& world)
{
    CONTROL_KEY keyControl = getKeyControl(key);

    Object& object = world.getObjectById(world.activeObjectId);
    auto& worldBounds = world.worldBounds;
    const double& metersPerChar = world.metersPerChar;
    auto& selectParam = world.objectInputInfo.selectParameter;

    if (keyControl == CONTROL_KEY::KEY_RESET_OBJECT){
        world.resetSimulation();
        return;
    }
    else if (object.isActionUsed && checkIsControlKeyMovement(keyControl)){
        return;
    }

    int &objCol = object.position.column;
    int& objRow = object.position.row;

    auto windowSize = getWindowSize();

    auto checkColInside = [&](int col){
        return col >= 0 && col < windowSize.ws_col;
    };
    auto checkRowInside = [&](int row){
        //NOTE: The floor is 1 row before the row count, we cannot go this far.
        return row >= 0 && row < windowSize.ws_row - 1;
    };


    switch (keyControl)
    {
        case CONTROL_KEY::KEY_MOVEUP:{
            if (checkRowInside(objRow - 1)){
                objRow -= 1;
                object.vectors.position.y += 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVEDOWN:{
            if (checkRowInside(objRow + 1)){
                objRow += 1;
                object.vectors.position.y -= 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVERIGHT:{
            if (checkColInside(objCol + 1)){
                objCol += 1;
                object.vectors.position.x += 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVELEFT:{
            if (checkColInside(objCol - 1)){
                objCol -= 1;
                object.vectors.position.x -= 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_ACTION:{
            world.startSimulation();
            break;
        }
        case CONTROL_KEY::KEY_LAUNCH:{
            auto& angle = object.launchInfo.launchAngleDeg;
            auto& velocity = object.launchInfo.velocity;

            if (angle <= 360 && angle >= 0 && velocity >= 0){
                object.launch(degreesToVector(angle) * velocity);

                if (!world.isSimulating){
                    world.startSimulation();
                }
            }

            break;
        }
        case CONTROL_KEY::KEY_SCALE_CONTROL_UP:{
            changeSelectParamValueOnInput(world, selectParam, true);
            break;
        }
        case CONTROL_KEY::KEY_SCALE_CONTROL_DOWN:{
            changeSelectParamValueOnInput(world, selectParam, false);
            break;
        }
        case CONTROL_KEY::KEY_SELECT_PARAM_LAUNCH_ANGLE:{
            changeSelectParam(world, SELECT_PARAMETER::LAUNCH_ANGLE);
            break;
        }
        case CONTROL_KEY::KEY_SELECT_PARAM_LAUNCH_VELOCITY:{
            changeSelectParam(world, SELECT_PARAMETER::LAUNCH_VELOCITY);
            break;
        }
    }
}

bool checkIsControlKeyMovement(CONTROL_KEY key)
{
    if (key == CONTROL_KEY::KEY_MOVEUP ||
        key == CONTROL_KEY::KEY_MOVEDOWN ||
        key == CONTROL_KEY::KEY_MOVERIGHT ||
        key == CONTROL_KEY::KEY_MOVELEFT)
    {
        return true;
    }
    return false;
}

void changeSelectParam(World& world, SELECT_PARAMETER param)
{
    auto& overlayText = world.overlayText;

    std::stringstream stream;
    stream << "selected parameter: ";

    if (param == SELECT_PARAMETER::LAUNCH_ANGLE){
        stream << "launch angle";
    }
    else if (param == SELECT_PARAMETER::LAUNCH_VELOCITY){
        stream << "launch velocity";
    }

    world.objectInputInfo.selectParameter = param;

    overlayText.text = stream.str();
    overlayText.displayUntil = getEpochAsDecimal() + 1.0;
    overlayText.shouldDisplay = true;
}

void changeSelectParamValueOnInput(World& world, SELECT_PARAMETER param,
                                   bool isPositive)
{
    Object& object = world.getObjectById(world.activeObjectId);

    std::stringstream stream;
    auto& inputInfo = world.objectInputInfo;

    if (inputInfo.selectParameter == SELECT_PARAMETER::LAUNCH_ANGLE){
        scaleActiveControl(world, isPositive ? 10 : -10);
        stream << "launch angle: " << inputInfo.objectLaunchAngle;
    }
    else if (inputInfo.selectParameter == SELECT_PARAMETER::LAUNCH_VELOCITY){
        scaleActiveControl(world, (isPositive ? 10 : -10) * world.metersPerChar);
        stream << "launch velocity: " << inputInfo.objectLaunchVelocity;
    }

    world.overlayText.text = stream.str();
    world.overlayText.displayUntil = getEpochAsDecimal() + 1;
    world.overlayText.shouldDisplay = true;
}
