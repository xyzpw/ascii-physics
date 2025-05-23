#include <ncurses.h>
#include <cmath>
#include "core/input/mouse_input.h"
#include "structs/world.h"
#include "structs/position.h"
#include "enums/object_type.h"
#include "enums/click_power.h"
#include "utils/sleep_utils.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"

MEVENT mouseEvent;
bool isLeftMouseActive = false; // is left mouse being held down

int idObjectFollowingMouse = -1;

bool hasShownLeftClickMsg = false; // release left mouse to place object

void clickButton1(World&, Position&);
void clickButton3(World&, Position); //right click
void releaseButton1(World&, Position&);
bool checkClickedMenuPanel(World&, Position&);
void clickPanelRow(World&, const int row);
bool useClickPowers(World&, Position&, bool isLeft);

void handleMouseClick(const char key, World& world)
{
    if (static_cast<char>(KEY_MOUSE) != key)
        return;
    if (getmouse(&mouseEvent) != OK)
        return;

    int buttonEvent = 0; // click event type

    const Position mousePos{.column=mouseEvent.x, .row=mouseEvent.y};

    if (mouseEvent.bstate & BUTTON1_PRESSED){
        isLeftMouseActive = true;
        buttonEvent = BUTTON1_PRESSED;
    }
    else if (mouseEvent.bstate & BUTTON1_RELEASED){
        isLeftMouseActive = false;
        buttonEvent = BUTTON1_RELEASED;
    }
    else if (mouseEvent.bstate & BUTTON3_PRESSED){
        buttonEvent = BUTTON3_PRESSED;
    }

    switch (buttonEvent)
    {
        case BUTTON1_PRESSED:{
            Position p = mousePos;
            clickButton1(world, p);
            break;
        }
        case BUTTON1_RELEASED:{
            Position p = mousePos;
            releaseButton1(world, p);
            break;
        }
        case BUTTON3_PRESSED:{
            clickButton3(world, mousePos);
            break;
        }
    }
}

// Handle left click, can be different depending on location.
void clickButton1(World& world, Position& pos)
{
    if (checkClickedMenuPanel(world, pos)){
        clickPanelRow(world, pos.row);
        return;
    }

    // Use click powers if enabled (then exit if used).
    if (useClickPowers(world, pos, true))
        return;

    const auto& defValues = world.defaultObjectValues;
    bool isAddingObject = true;
    bool posHasObject = false;

    for (const auto& it : world.objects){
        if (it.position == pos){
            idObjectFollowingMouse = it.id;
            isAddingObject = false;
            posHasObject = true;
        }
    }
    for (const auto& it : world.obstacles){
        if (it.position == pos){
            world.setOverlayText("cannot place object here");
            return;
        }
    }

    if (isAddingObject){
        world.addObject(OBJECT_TYPE::OBJECT_BALL, defValues.objectMass, pos);
        if (world.objectInputInfo.isSlingshotMode){
            idObjectFollowingMouse = world.activeObjectId;
            world.getObjectById(idObjectFollowingMouse).isFrozen = true;
        }
        return;
    }
    else if (posHasObject && world.checkObjectIdExists(idObjectFollowingMouse)){
        Object& ob = world.getObjectById(idObjectFollowingMouse);
        world.highlightObjectById(idObjectFollowingMouse, 10);

        if (!hasShownLeftClickMsg){
            world.setOverlayText("release mouse to place object", 1.5);
            hasShownLeftClickMsg = true;
        }
    }
}

// Handle right mouse click.
void clickButton3(World& world, Position pos)
{
    const bool isIdValid = world.checkObjectIdExists(idObjectFollowingMouse);
    const bool hasPowers = world.clickPower != CLICK_POWER_NONE;

    if (!isLeftMouseActive && hasPowers){
        useClickPowers(world, pos, false);
        return;
    }
    else if (!isLeftMouseActive){
        return;
    }

    if (isIdValid){
        Object& obj = world.getObjectById(idObjectFollowingMouse);
        obj.isFrozen = false;
        obj.highlightInfo.isHighlighted = false;
    }

    idObjectFollowingMouse = -1;
}

// Handle release left click.
void releaseButton1(World& world, Position& pos)
{
    bool isPosValid = true;
    bool isIdValid = world.checkObjectIdExists(idObjectFollowingMouse);
    const bool& isSlingshot = world.objectInputInfo.isSlingshotMode;
    Vector2D startVecPos{-1,-1};
    Object* object = nullptr;

    for (const auto& it : world.objects){
        if (it.position == pos){
            if (isIdValid && it.id == idObjectFollowingMouse){
                world.activeObjectId = idObjectFollowingMouse;
                world.activeEntityId = idObjectFollowingMouse;
                world.getObjectById(idObjectFollowingMouse).isFrozen = false;
                world.highlightObjectById(idObjectFollowingMouse);
                idObjectFollowingMouse = -1;
                return;
            }

            isPosValid = false;
        }
    }
    for (const auto& it : world.obstacles){
        if (it.position == pos){
            isPosValid = false;
        }
    }

    if (isIdValid){
        object = &world.getObjectById(idObjectFollowingMouse);
    }

    if (isPosValid && isIdValid){
        startVecPos = object->vectors.position;
        object->position = pos;
        object->vectors.position = positionToVector(pos, world.metersPerChar);
        object->highlightInfo.isHighlighted = false;
    }
    else if (isIdValid){
        object->highlightInfo.isHighlighted = false;
        object->isFrozen = false;
        world.setOverlayText("cannot place object here");
    }

    if (isIdValid && isPosValid && isSlingshot){
        object->slingshot(startVecPos);
        if (!world.isSimulating) world.startSimulation();
    }

    idObjectFollowingMouse = -1;
}

// Check if any button was clicked on the menu panel.
bool checkClickedMenuPanel(World& world, Position& pos)
{
    auto& panel = world.menuPanel;

    if (!panel.isOpen)
        return false;

    if (pos.column < panel.colMin || pos.column > panel.colMax)
        return false;

    for (auto& it : panel.items){
        if (it.second.row == pos.row)
            return true;
    }

    return false;
}

// Find what item is at the specified row and activate it.
void clickPanelRow(World& world, const int row)
{
    auto& panel = world.menuPanel;

    for (auto& it : panel.items)
    {
        if (row == it.second.row){
            world.clickPanelItem(it.first);
            break;
        }
    }
}

// Use click power if enabled, returns true if the power was successfully used.
bool useClickPowers(World& world, Position& pos, bool isLeft)
{
    bool usedPower = false;

    switch (world.clickPower){
        case CLICK_POWER_NONE:{
            return false;
        }
        case CLICK_POWER_REPEL_ATTRACT:{
            world.useRepelAttractClick(pos, isLeft);
            usedPower = true;
            break;
        }
    }

    if (usedPower && !world.isSimulating) world.startSimulation();

    return usedPower;
}
