#include <thread>
#include <algorithm>
#include "structs/object.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "utils/random_utils.h"
#include "constants/physics_constants.h"
#include "constants/object_char_constants.h"
#include "core/physics/object_physics.h"

const double PI = 3.141592653589793238;

void configureObjectProperties(Object&);

Object::Object(OBJECT_TYPE type, double kg)
{
    this->type = type;
    this->mass = kg;

    this->terminalVelocity = calculateTerminalVelocity(*this);
    this->vectors.acceleration = Vector2D{0.0, -GRAVITY_ACCELERATION};

    this->id = randInt();
}

void Object::resetVectors()
{
    this->vectors.position = positionToVector(this->position) * this->mLength;
    this->vectors.velocity = Vector2D{0.0, 0.0};
    this->vectors.acceleration = Vector2D{0.0, -GRAVITY_ACCELERATION};
}

void Object::resetPositionToCenter(double charSize)
{
    Position pos = getCenterPosition();
    Vector2D vec = positionToVector(getCenterPosition()) * charSize;
    pos = vectorToPosition(vec);

    this->position = pos;
}

void Object::resetActionStates()
{
    this->isActionUsed = false;
}

void Object::resetPhysicsProperties(double charSize)
{
    switch (this->type)
    {
        case OBJECT_TYPE::OBJECT_BALL: {
            this->dragCoefficient = DRAG_COEFFICIENT_BALL;
            this->crossSectionalArea = (charSize/2.0) * (charSize/2.0) * PI;
            this->terminalVelocity = calculateTerminalVelocity(*this);
        }
    }
}

void Object::addTrail(Position pos)
{
    // Return if trail already exists.
    for (const auto& tr : trails){
        if (tr.position == pos)
            return;
    }

    trails.push_back(Trail{.position=pos});
}

// Update trail values for each frame step.
void Object::updateTrails()
{
    for (auto& tr : trails)
    {
        ++tr.frameAge;
        if (!tr.isOld && TRAIL_FRAME_AGE_OLD < tr.frameAge){
            tr.isOld = true;
            tr.displayChar = ".";
            tr.colorPairNum = PAIR_NUM_DARK_GRAY_FG;
        }
    }
}

void Object::removeExpiredTrails()
{
    auto removalIt = std::remove_if(trails.begin(), trails.end(), [](Trail& t){
        return TRAIL_FRAME_AGE_MAX < t.frameAge;
    });

    trails.erase(removalIt, trails.end());
}

void Object::launch(Vector2D velocity)
{
    this->vectors.velocity += velocity;
}

// Slingshots object toward the resting position.
void Object::slingshot(Vector2D restPos)
{
    double pe = computeElasticPotentialEnergy(vectors.position, restPos);
    double vel = kineticEnergyToVelocity(pe, mass);

    isFrozen = false;

    this->launch((restPos - vectors.position) * vel);
}
