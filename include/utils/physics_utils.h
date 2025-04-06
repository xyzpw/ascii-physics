#pragma once

#include "structs/object.h"
#include "structs/vector2d.h"
#include "enums/object_type.h"

double calculateTerminalVelocity(Object&);
double calculateDragForce(double velocity, double dragCoeff, double area);
double calculateVelocityFinal(double velocity, double acceleration, double t);
double calculateDisplacement(double velocity, double acceleration, double t);
Vector2D degreesToVector(double deg);
