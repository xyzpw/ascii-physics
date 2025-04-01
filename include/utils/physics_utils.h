#pragma once

#include "structs/object.h"
#include "enums/object_type.h"

double calculateTerminalVelocity(Object&);
double calculateVelocityFinal(double velocity, double acceleration, double t);
double calculateDisplacement(double velocity, double acceleration, double t);
