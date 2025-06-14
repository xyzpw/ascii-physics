#include <thread>
#include <sstream>
#include <cctype>
#include <string>
#include "core/input/keyboard_input.h"
#include "core/input/object_control_scale.h"
#include "core/input/multiselect_input.h"
#include "core/physics/object_physics.h"
#include "structs/object.h"
#include "structs/world.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "utils/epoch_utils.h"
#include "constants/control_keys.h"

const double WIND_GUST_DUR = 0.25; //duration of world wind gust function

bool checkIsControlKeyMovement(CONTROL_KEY);
void changeSelectParam(World&, SELECT_PARAMETER);
void changeSelectParamValueOnInput(World&, SELECT_PARAMETER, bool isPositive);
void handleCustomInput(const char key, World& world);
void setParamValueFromCustomInput(World&);

void handleKeyPress(const char key, World& world)
{
    if (world.objectInputInfo.isTakingInput){
        handleCustomInput(key, world);
        return;
    }

    CONTROL_KEY keyControl = getKeyControl(key);

    Object& object = world.getObjectById(world.activeObjectId);
    const double& metersPerChar = world.metersPerChar;
    auto& selectParam = world.objectInputInfo.selectParameter;
    auto& objInputInfo = world.objectInputInfo;

    bool isActiveIdObstacle = world.checkObstacleIdExists(world.activeEntityId);

    if (keyControl == CONTROL_KEY::KEY_RESET_OBJECT){
        world.resetSimulation();
        return;
    }
    else if (!objInputInfo.isWindGustMode && world.isSimulating && checkIsControlKeyMovement(keyControl)){
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

    auto useMovement = [&](int dcol, int drow){
        if (isActiveIdObstacle){
            moveSelectedEntities(world, dcol, drow);
            return;
        }

        Position& pos = world.getActiveEntityPosition();
        Vector2D& vec = world.getActiveEntityVectorPosition();

        if (dcol != 0 && checkColInside(pos.column + dcol)){
            pos.column += dcol;
            vec.x += dcol * metersPerChar;
        }
        if (drow != 0 && checkRowInside(pos.row + drow)){
            pos.row += drow;
            vec.y += -drow * metersPerChar;
        }
    };

    // Use wind gust function (starts simulation if not already simulating).
    //NOTE: Direction order 1 is right or up, -1 is left or down.
    //NOTE: Wind gust will not be used if disabled (returns true on success).
    auto useWindGust = [&](double xDir = 0, double yDir = 0){
        if (!objInputInfo.isWindGustMode) return false;
        const double& vel = world.windGustVelocity;
        Vector2D vec{xDir, yDir};
        world.useWindGust(vec, WIND_GUST_DUR);
        if (!world.isSimulating) world.startSimulation();
        return true;
    };

    switch (keyControl)
    {
        case CONTROL_KEY::KEY_MOVEUP:{
            if (useWindGust(0, 1)) break;
            useMovement(0, -1);
            break;
        }
        case CONTROL_KEY::KEY_MOVEDOWN:{
            if (useWindGust(0, -1)) break;
            useMovement(0, 1);
            break;
        }
        case CONTROL_KEY::KEY_MOVERIGHT:{
            if (useWindGust(1, 0)) break;
            useMovement(1, 0);
            break;
        }
        case CONTROL_KEY::KEY_MOVELEFT:{
            if (useWindGust(-1, 0)) break;
            useMovement(-1, 0);
            break;
        }
        case CONTROL_KEY::KEY_ACTION:{
            world.startSimulation();
            break;
        }
        case CONTROL_KEY::KEY_LAUNCH:{
            auto& angle = world.objectInputInfo.objectLaunchAngle;
            auto& velocity = world.objectInputInfo.objectLaunchVelocity;

            if (angle <= 360 && angle >= 0 && velocity >= 0){
                object.launch(degreesToVector(angle) * velocity);

                if (!world.isSimulating){
                    world.startSimulation();
                }
            }

            break;
        }
        case CONTROL_KEY::KEY_SCALE_CONTROL_UP:{
            if (isActiveIdObstacle){
                resizeSelectedObstacleY(world, 1);
                break;
            }

            changeSelectParamValueOnInput(world, selectParam, true);
            break;
        }
        case CONTROL_KEY::KEY_SCALE_CONTROL_DOWN:{
            if (isActiveIdObstacle){
                resizeSelectedObstacleY(world, -1);
                break;
            }
            changeSelectParamValueOnInput(world, selectParam, false);
            break;
        }
        case CONTROL_KEY::KEY_OBJECT_SELECT_NEXT:
        case CONTROL_KEY::KEY_OBJECT_SELECT_PREV:
            selectObjectNextOrPrev(world, keyControl);
            world.highlightObjectById(world.activeObjectId, 0.5);
            break;
        case CONTROL_KEY::KEY_SELECT_PARAM_LAUNCH_ANGLE:{
            changeSelectParam(world, SELECT_PARAMETER::LAUNCH_ANGLE);
            break;
        }
        case CONTROL_KEY::KEY_SELECT_PARAM_LAUNCH_VELOCITY:{
            changeSelectParam(world, SELECT_PARAMETER::LAUNCH_VELOCITY);
            break;
        }
        case CONTROL_KEY::KEY_SELECT_PARAM_MASS:{
            changeSelectParam(world, SELECT_PARAMETER::OBJECT_MASS);
            break;
        }
        case CONTROL_KEY::KEY_NEW_OBJECT:{
            world.addObject(
                OBJECT_TYPE::OBJECT_BALL, world.defaultObjectValues.objectMass
            );
            break;
        }
        case CONTROL_KEY::KEY_NEW_OBSTACLE:{
            if (!world.isSimulating){
                world.addObstacle();
                break;
            }
            world.setOverlayText("cannot add obstacle while simulating", 2);
            break;
        }
        case CONTROL_KEY::KEY_UNDO_SPAWN:{
            if (world.entityIdSpawnOrder.size() <= 1){
                break;
            }
            world.undoSpawn();
            break;
        }
        case CONTROL_KEY::KEY_CUSTOM_INPUT:{
            std::string txt = "input " + selectParamToStr(selectParam) + ":";
            world.promptCustomInput(txt);
            break;
        }
        case CONTROL_KEY::KEY_SLINGSHOT_MODE:{
            objInputInfo.isSlingshotMode = objInputInfo.isSlingshotMode ? false : true;

            std::string str = objInputInfo.isSlingshotMode ? "enabled" : "disabled";

            world.setOverlayText(str + " slingshot mode");
            break;
        }
        case CONTROL_KEY::KEY_OPEN_MENU_PANEL:{
            world.menuPanel.isOpen = world.menuPanel.isOpen ? false : true;
            break;
        }
    }
}

void handleCustomInput(const char key, World& world)
{
    auto& inputInfo = world.objectInputInfo;
    bool& isTakingInput = world.objectInputInfo.isTakingInput;
    double& takeInputUntil = world.objectInputInfo.takeInputUntil;
    std::string& customInputValue = world.objectInputInfo.customInputValue;

    const bool hasInputFinished = getEpochAsDecimal() > takeInputUntil;
    const bool isInputValid = std::isdigit(key) || key == '.' || key == '-';

    if (hasInputFinished){
        try {
            setParamValueFromCustomInput(world);
            world.setOverlayText("value set");
        } catch (...){
            world.setOverlayText("invalid input; value not set", 2);
        }
        isTakingInput = false;
        return;
    }

    if (!isInputValid && key != static_cast<char>(KEY_BACKSPACE)){
        return;
    }

    if (key == static_cast<char>(KEY_BACKSPACE)){
        customInputValue.pop_back();
    }
    else {
        customInputValue.push_back(key);
    }

    takeInputUntil = getEpochAsDecimal() + 1;

    std::stringstream stream;
    stream << "input ";
    stream << selectParamToStr(inputInfo.selectParameter);
    stream << ": " << customInputValue;

    world.setOverlayText(stream.str());
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
    else if (param == SELECT_PARAMETER::OBJECT_MASS)
    {
        stream << "object mass";
    }

    world.objectInputInfo.selectParameter = param;

    world.setOverlayText(stream.str(), 1);
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
    else if (inputInfo.selectParameter == SELECT_PARAMETER::OBJECT_MASS){
        scaleActiveControl(world, (isPositive ? 0.1 : -0.1) * object.mass);
        stream << "object mass: " << object.mass;
    }

    world.overlayText.text = stream.str();
    world.overlayText.displayUntil = getEpochAsDecimal() + 1;
    world.overlayText.shouldDisplay = true;
}

// Set the active select parameter to the value of custom input value.
void setParamValueFromCustomInput(World& world)
{
    Object& object = world.getObjectById(world.activeObjectId);
    auto& inputInfo = world.objectInputInfo;
    auto& activeParam = world.objectInputInfo.selectParameter;
    std::string& inputValue = world.objectInputInfo.customInputValue;

    double newVal = std::stod(inputValue);
    inputValue = "";

    switch (inputInfo.selectParameter)
    {
        case SELECT_PARAMETER::OBJECT_MASS:{
            if (0 >= newVal)
                return;
            object.mass = newVal;
            return;
        }
        case SELECT_PARAMETER::LAUNCH_VELOCITY:{
            if (0 > newVal)
                return;
            inputInfo.objectLaunchVelocity = newVal;
            return;
        }
        case SELECT_PARAMETER::LAUNCH_ANGLE:{
            if (0 > newVal || 360 < newVal)
                return;
            inputInfo.objectLaunchAngle = newVal;
            return;
        }
        case SELECT_PARAMETER::WIND_SPEED:{
            world.windVelocity = newVal;
            return;
        }
    }
}
