#pragma once

#include "structs/object.h"
#include "structs/world.h"

void updateObjectPhysicsYAxis(World&, Object&, double t);
void updateObjectPhysicsXAxis(World&, Object&, double tDelta);
void applyWindToObject(World&, Object&, double tDelta);
void updateObjectVectors(World&, Object&, double tDelta);
void simulateObjectsInWorld(World&);
