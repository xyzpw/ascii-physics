#include <stdexcept>
#include <sstream>
#include <thread>
#include "structs/world.h"
#include "structs/object.h"
#include "enums/object_type.h"
#include "utils/screen_utils.h"
#include "core/physics/object_physics.h"

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

    this->objects.push_back(newObject);
    this->resetObjectById(newObject.id);
    this->activeObjectId = newObject.id;
}

void World::removeAllObjects()
{
    this->objects = {};
}
