#include <stdexcept>
#include <sstream>
#include <thread>
#include <algorithm>
#include <mutex>
#include <cmath>
#include "structs/world.h"
#include "structs/object.h"
#include "structs/obstacle.h"
#include "enums/object_type.h"
#include "enums/click_power.h"
#include "utils/screen_utils.h"
#include "utils/sleep_utils.h"
#include "utils/epoch_utils.h"
#include "utils/random_utils.h"
#include "utils/physics_utils.h"
#include "core/physics/object_physics.h"

using PANEL_ACTION = PANEL_ITEM_ACTION;
using PANEL_KEY = PANEL_ITEM_KEY;

std::mutex undoSpawnMutex;

Position getNewObjectPosition(World&);
void useStartupCommands(World&);

World::World()
{
    this->setWorldBounds();
    menuPanel.colMax = getWindowSize().ws_col - 1;
    menuPanel.addItem(PANEL_KEY::QUIT_OR_RESET, PANEL_ACTION::QUIT, "quit");
    menuPanel.addItem(PANEL_KEY::SLINGSHOT, PANEL_ACTION::SLINGSHOT, "slingshot");
    menuPanel.addItem(PANEL_KEY::REPEL_ATTRACT, PANEL_ACTION::REPEL_ATTRACT, "repel/attract");
    menuPanel.addItem(PANEL_KEY::IMP_VERT, PANEL_ACTION::IMP_VERT, "vertical impulse");
    menuPanel.addItem(PANEL_KEY::WIND_GUST, PANEL_ACTION::WIND_GUST, "wind gust");
    menuPanel.addItem(PANEL_KEY::WIND_SPEED, PANEL_ACTION::EDIT_WIND_SPEED, "edit wind speed");
    menuPanel.adjustColMin();
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

    activeEntityId = activeObjectId;

    menuPanel.items.at(PANEL_KEY::QUIT_OR_RESET).text = "reset";
    menuPanel.adjustColMin();
    menuPanel.items.at(PANEL_KEY::QUIT_OR_RESET).action = PANEL_ACTION::RESET;

    useStartupCommands(*this);

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

    menuPanel.items.at(PANEL_KEY::QUIT_OR_RESET).text = "quit";
    menuPanel.adjustColMin();
    menuPanel.items.at(PANEL_KEY::QUIT_OR_RESET).action = PANEL_ACTION::QUIT;
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

bool World::checkObstacleIdExists(int id)
{
    for (const auto& ob : obstacles){
        if (id == ob.id)
            return true;
    }
    return false;
}

Position& World::getEntityPositionById(int id)
{
    if (checkObjectIdExists(id)) return getObjectById(id).position;
    else if (checkObstacleIdExists(id)) return getObstacleById(id).position;

    throw std::runtime_error("object id does not exist");
}

Vector2D& World::getEntityVectorPositionById(int id)
{
    if (checkObjectIdExists(id))
        return getObstacleById(id).vectors.position;
    else if (checkObstacleIdExists(id))
        return getObstacleById(id).vectors.position;

    throw std::runtime_error("object id does not exist");
}

int World::getEntityIdByPosition(const Position& pos)
{
    for (const auto& ob : objects){
        if (ob.position == pos)
            return ob.id;
    }
    for (const auto& ob : obstacles){
        if (ob.position == pos){
            return ob.id;
        }
    }

    return -1;
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

void World::promptCustomInput(std::string text, double duration)
{
    this->setOverlayText(text, duration);
    objectInputInfo.isTakingInput = true;
    objectInputInfo.takeInputUntil = getEpochAsDecimal() + duration;
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
    entityMultiselect.clear();
}

void World::addObstacle(Position pos)
{
    Obstacle obstacle{.id=randInt()};

    if (pos.column == -1 && pos.row == -1){
        obstacle.position = getNewObjectPosition(*this);
        if (!checkPositionInsideDisplay(obstacle.position))
            return;
        entityMultiselect.clear();
    }
    else {
        for (auto& it : this->obstacles){
            if (it.position == pos)
                return;
        }
        obstacle.position = pos;
    }

    obstacle.vectors.position = positionToVector(obstacle.position, metersPerChar);
    obstacle.mLength = this->metersPerChar;

    this->obstacles.push_back(obstacle);
    if (entityMultiselect.selectedIds.size() == 0)
        this->activeEntityId = obstacle.id;
    this->entityIdSpawnOrder.push_back(obstacle.id);

    entityMultiselect.add(obstacle.id);
}

void World::removeEntityById(const int id)
{
    auto& order = this->entityIdSpawnOrder;
    auto& mIds = entityMultiselect.selectedIds;

    auto rmIfExists = [&](auto& vec){
        auto it = std::remove_if(
            vec.begin(), vec.end(), [&](auto& t) { return id == t.id; }
        );
        vec.erase(it, vec.end());
        mIds.erase(std::remove(mIds.begin(), mIds.end(), id), mIds.end());
    };

    rmIfExists(this->objects);
    rmIfExists(this->obstacles);
}

void World::removeEntityByPosition(const Position pos)
{
    removeEntityById(getEntityIdByPosition(pos));
}

void World::removeAllObjects()
{
    this->objects = {};
    entityMultiselect.clear();
}

void World::removeAllObstacles()
{
    this->obstacles = {};
    entityMultiselect.clear();
}

// Remove the the last spawned entity.
void World::undoSpawn()
{
    // Cancel if selected multiselect is active.
    for (auto id : entityMultiselect.selectedIds){
        if (id == activeEntityId)
            return;
    }

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

void World::useRepelAttractClick(Position& clickPos, bool isRepel)
{
    const double initVel = 100;
    const Vector2D clickVec = positionToVector(clickPos, metersPerChar);

    for (auto& ob : objects){
        const Vector2D& obVecPos = ob.vectors.position;

        double dist = (ob.vectors.position - clickVec).getMagnitude();
        if (dist <= 0) continue;
        double vel = initVel * calculateInverseSquareLaw(dist);
        Vector2D dir = isRepel ? obVecPos - clickVec : clickVec - obVecPos;
        ob.launch(dir * vel);
    }
}

// Increase/decrease objects y-axis velocity.
void World::useImpulseVertical(bool isUp)
{
    double hCeil = worldBounds.ceiling - worldBounds.floor;
    hCeil *= 0.67;
    const double acc = (isUp ? -1.0 : 1.0) * GRAVITY_ACCELERATION;
    double initVel = std::sqrt(2 * GRAVITY_ACCELERATION * hCeil);
    if (!isUp) initVel = -initVel;

    for (auto& ob : objects){
        ob.vectors.velocity.y += initVel;
    }
}

// Apply wind gust against all objects.
void World::useWindGust(Vector2D direction, double duration)
{
    for (auto& ob : objects)
    {
        double fDrag = calculateDragForce(
            windGustVelocity,
            ob.dragCoefficient,
            ob.crossSectionalArea
        );
        double acc = fDrag / ob.mass;
        ob.vectors.velocity += direction * (acc * duration);
    }
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

    auto checkPow = [&](CLICK_POWER p) { return this->clickPower == p; };

    std::stringstream statusStream;

    switch (item->action)
    {
        case PANEL_ACTION::QUIT:{
            this->quit();
            break;
        }
        case PANEL_ACTION::RESET:{
            this->resetSimulation();
            break;
        }
        case PANEL_ACTION::SLINGSHOT:{
            bool& slingshot = objectInputInfo.isSlingshotMode;
            slingshot = slingshot ? false : true;

            statusStream << "slingshot mode ";
            statusStream << (slingshot ? "enabled" : "disabled");
            break;
        }
        case PANEL_ACTION::REPEL_ATTRACT: {
            bool isRep = checkPow(CLICK_POWER_REPEL_ATTRACT);
            clickPower = isRep ? CLICK_POWER_NONE : CLICK_POWER_REPEL_ATTRACT;

            statusStream << "repel/attract click ";
            statusStream << (isRep ? "disabled" : "enabled");
            break;
        }
        case PANEL_ACTION::IMP_VERT: {
            bool isImp = checkPow(CLICK_POWER_IMP_VERT);
            clickPower = isImp ? CLICK_POWER_NONE : CLICK_POWER_IMP_VERT;

            statusStream << "vertical impulse ";
            statusStream << (isImp ? "disabled" : "enabled");
            break;
        }
        case PANEL_ACTION::WIND_GUST:{
            bool& wind = objectInputInfo.isWindGustMode;
            wind = wind ? false : true;

            statusStream << "wind gust mode ";
            statusStream << (wind ? "enabled" : "disabled");
            break;
        }
        case PANEL_ACTION::EDIT_WIND_SPEED:{
            objectInputInfo.selectParameter = SELECT_PARAMETER::WIND_SPEED;
            promptCustomInput("input wind speed:");
            break;
        }
    }

    if (statusStream.str().length() > 0){
        setOverlayText(statusStream.str());
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

void useStartupCommands(World& world)
{
    auto& commands = world.startupCommands;
    auto& object = world.getObjectById(world.activeObjectId);

    object.launch(commands.launch);
    world.windVelocity = commands.windVelocity;
}
