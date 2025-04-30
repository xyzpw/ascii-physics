#include <stdexcept>
#include <sstream>
#include <thread>
#include "structs/world.h"
#include "structs/object.h"
#include "enums/object_type.h"
#include "utils/screen_utils.h"
#include "utils/sleep_utils.h"
#include "utils/epoch_utils.h"
#include "core/physics/object_physics.h"

void highlightObject(Object&, double seconds);
Position getNewObjectPosition(World&);

World::World()
{
    this->setWorldBounds();
}

void World::startSimulation()
{
    if (this->isSimulating){
        return;
    }
    this->isSimulating = true;
    std::thread simThread(simulateObjectsInWorld, std::ref(*this));
    simThread.detach();
}

void World::stopSimulation()
{
    this->isSimulating = false;
}

void World::resetSimulation()
{
    if (!this->isSimulating){
        return;
    }

    this->isSimulating = false;

    this->removeAllObjects();
    this->addObject(
        this->defaultObjectValues.objectType,
        this->defaultObjectValues.objectMass
    );
    this->activeObjectId = this->objects.at(0).id;
}

Object& World::getObjectById(int id)
{
    for (auto& obj : this->objects){
        if (obj.id == id)
            return obj;
    }

    std::stringstream errMsg;
    errMsg << "no object with id " << id << " exists";

    throw std::invalid_argument(errMsg.str());
}

void World::resetObjectById(int id)
{
    Object& object = this->getObjectById(id);

    object.resetActionStates();
    object.resetPositionToCenter(this->metersPerChar);
    object.resetVectors();
    object.resetPhysicsProperties(this->metersPerChar);
}

void World::highlightObjectById(int id, double duration)
{
    Object& object = this->getObjectById(id);
    object.highlightInfo.highlightUntil = getEpochAsDecimal() + duration;
    object.highlightInfo.isHighlighted = true;
}

void World::hitFlashObjectById(int id, double duration)
{
    Object& object = this->getObjectById(id);
    object.highlightInfo.hitFlashingUntil = getEpochAsDecimal() + duration;
    object.highlightInfo.isHitFlashing = true;
}

void World::setWorldBounds()
{
    auto windowSize = getWindowSize();

    auto& bounds = this->worldBounds;

    bounds.rightWall = (windowSize.ws_col - 1) * this->metersPerChar;
    bounds.leftWall = 0;
    bounds.ceiling = (windowSize.ws_row - 1) * this->metersPerChar;
    bounds.floor = 0;
}

void World::addObject(OBJECT_TYPE type, double kg)
{
    Object newObject(type, kg);

    newObject.mass = kg;
    newObject.dragCoefficient = this->defaultObjectValues.dragCoefficient;
    newObject.crossSectionalArea = this->defaultObjectValues.crossSectionalArea;
    newObject.coefficientOfRestitution =
              this->defaultObjectValues.coefficientOfRestitution;
    newObject.mLength = this->metersPerChar;

    newObject.position = getNewObjectPosition(*this);
    if (!checkPositionInsideDisplay(newObject.position))
        return;
    newObject.resetActionStates();
    newObject.resetVectors();
    newObject.resetPhysicsProperties(this->metersPerChar);
    this->objects.push_back(newObject);
    this->activeObjectId = newObject.id;
}

void World::removeAllObjects()
{
    this->objects = {};
}

void highlightObject(Object& object, double seconds)
{
    if (object.highlightInfo.isHighlighted)
        return;

    object.highlightInfo.isHighlighted = true;
    sleepCurrentThread(static_cast<int>(seconds * 1e+3));
    object.highlightInfo.isHighlighted = false;
}

Position getNewObjectPosition(World& world)
{
    Position center = getCenterPosition();

    if (world.objects.size() == 1)
        return center;

    for (int row = center.row - 1; row > 0; --row)
    {
        const Position pos{center.column, row};

        bool isOpen = true;

        for (const auto it : world.objects)
        {
            if (it.position == pos || !checkPositionInsideDisplay(pos)){
                isOpen = false;
                break;
            }
        }

        if (isOpen)
            return pos;
    }

    return Position{-1, -1};
}
