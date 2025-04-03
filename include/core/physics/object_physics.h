#pragma once

#include "structs/object.h"

void updateObjectPhysicsYAxis(Object&, double t);
void updateObjectPhysicsXAxis(Object&, double tDelta);
void updateObjectVectors(Object&, double tDelta);
void simulateObjectMotion(Object&);
