#include <stdexcept>
#include <sstream>
#include <thread>
#include <algorithm>
#include <mutex>
#include "structs/world.h"
#include "structs/object.h"
#include "structs/obstacle.h"
#include "enums/object_type.h"
#include "utils/screen_utils.h"
#include "utils/sleep_utils.h"
#include "utils/epoch_utils.h"
#include "utils/random_utils.h"
#include "core/physics/object_physics.h"

std::mutex undoSpawnMutex;

Position getNewObjectPosition(World&);

World::World()
{
    this->setWorldBounds();
    this->menuPanel.items.insert({
        PANEL_ITEM_KEY::QUIT,
        MenuPanelItem{PANEL_ITEM_ACTION::QUIT, 0, "quit"}
    });
    this->menuPanel.colMax = getWindowSize().ws_col - 1;
    this->menuPanel.colMin = menuPanel.colMax - 5;
}

void World::quit()
{
    this->isSimulating = false;
    this->hasQuit = true;
}

void World::startSimulation()
{
    if (this->isSimulating){
        return;
    }
    this->isSimulating = true;
    this->menuPanel.items.at(PANEL_ITEM_KEY::QUIT).text = "reset";
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
    this->removeAllObstacles();
    this->entityIdSpawnOrder = {};
    this->addObject(
        this->defaultObjectValues.objectType,
        this->defaultObjectValues.objectMass
    );
    this->activeObjectId = this->objects.at(0).id;
    this->activeEntityId = this->objects.at(0).id;

    this->menuPanel.items.at(PANEL_ITEM_KEY::QUIT).text = "quit";
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

bool World::checkObjectIdExists(int id)
{
    for (auto& obj : this->objects){
        if (obj.id == id)
            return true;
    }
    return false;
}

Obstacle& World::getObstacleById(int id)
{
    for (auto& ob : this->obstacles){
        if (ob.id == id)
            return ob;
    }

    std::stringstream errMsg;
    errMsg << "no obstacle with id " << id << " exists";

    throw std::invalid_argument(errMsg.str());
}

Vector2D& World::getActiveEntityVectorPosition()
{
    for (auto& ent : this->objects){
        if (ent.id == this->activeEntityId)
            return ent.vectors.position;
    }
    for (auto& ent : this->obstacles){
        if (ent.id == this->activeEntityId)
            return ent.vectors.position;
    }

    throw std::invalid_argument("no active entity exists for vector position");
}

Position& World::getActiveEntityPosition()
{
    for (auto& ent : this->objects){
        if (ent.id == this->activeEntityId)
            return ent.position;
    }
    for (auto& ent : this->obstacles){
        if (ent.id == this->activeEntityId)
            return ent.position;
    }

    throw std::invalid_argument("no active entity exists for position");
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

void World::setOverlayText(std::string text, double duration)
{
    this->overlayText.text = text;
    this->overlayText.displayUntil = getEpochAsDecimal() + duration;
    this->overlayText.shouldDisplay = true;
}

void World::addObject(OBJECT_TYPE type, double kg, Position pos)
{
    Object newObject(type, kg);

    newObject.mass = kg;
    newObject.dragCoefficient = this->defaultObjectValues.dragCoefficient;
    newObject.crossSectionalArea = this->defaultObjectValues.crossSectionalArea;
    newObject.coefficientOfRestitution =
              this->defaultObjectValues.coefficientOfRestitution;
    newObject.mLength = this->metersPerChar;

    newObject.position = getNewObjectPosition(*this);
    if (pos.column == -1 && pos.row == -1){
        newObject.position = getNewObjectPosition(*this);
        if (!checkPositionInsideDisplay(newObject.position))
            return;
    }
    else{
        for (auto& it : this->objects){
            if (it.position == pos)
                return;
            newObject.position = pos;
        }
    }

    newObject.resetActionStates();
    newObject.resetVectors();
    newObject.resetPhysicsProperties(this->metersPerChar);

    this->objects.push_back(newObject);
    this->activeEntityId = newObject.id;
    this->activeObjectId = newObject.id;
    this->entityIdSpawnOrder.push_back(newObject.id);
}

void World::addObstacle()
{
    Obstacle obstacle{.id=randInt()};

    obstacle.position = getNewObjectPosition(*this);
    if (!checkPositionInsideDisplay(obstacle.position))
        return;
    obstacle.vectors.position = positionToVector(obstacle.position) * metersPerChar;
    obstacle.mLength = this->metersPerChar;

    this->obstacles.push_back(obstacle);
    this->activeEntityId = obstacle.id;
    this->entityIdSpawnOrder.push_back(obstacle.id);
}

void World::removeEntityById(const int id)
{
    auto& order = this->entityIdSpawnOrder;

    auto rmIfExists = [&](auto& vec){
        auto it = std::remove_if(
            vec.begin(), vec.end(), [&](auto& t) { return id == t.id; }
        );
        vec.erase(it, vec.end());
    };

    rmIfExists(this->objects);
    rmIfExists(this->obstacles);
}

void World::removeAllObjects()
{
    this->objects = {};
}

void World::removeAllObstacles()
{
    this->obstacles = {};
}

// Remove the the last spawned entity.
void World::undoSpawn()
{
    std::lock_guard<std::mutex> lock(undoSpawnMutex);

    int size = this->entityIdSpawnOrder.size();

    this->removeEntityById(this->entityIdSpawnOrder.at(size - 1));
    this->entityIdSpawnOrder.pop_back();
    --size;

    this->activeEntityId = this->entityIdSpawnOrder.at(size - 1);

    for (auto index = size - 1; index >= 0; --index){
        const int& id = this->entityIdSpawnOrder.at(index);

        if (this->checkObjectIdExists(id)){
            this->activeObjectId = id;
            break;
        }
    }
}

Position getNewObjectPosition(World& world)
{
    Position center = getCenterPosition();

    if (world.objects.size() == 1 && world.obstacles.size() == 1)
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
        for (const auto& it : world.obstacles)
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

void World::clickPanelItem(PANEL_ITEM_KEY key)
{
    MenuPanelItem* item = nullptr;

    for (auto& it : this->menuPanel.items){
        if (it.first == key)
            item = &it.second;
    }

    if (nullptr == item)
        return;

    switch (item->action)
    {
        case PANEL_ITEM_ACTION::QUIT:{
            if (this->isSimulating){
                this->resetSimulation();
                item->text = "quit";
                break;
            }
            this->quit();
            break;
        }
    }
}
