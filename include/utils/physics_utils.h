#pragma once

#include "structs/object.h"
#include "enums/object_type.h"

double calculateFreeFallDistanceAtTime(Object&, double seconds);
double calculateFreeFallVelocityAtTime(Object&, double seconds);
double calculateTerminalVelocity(Object&);
