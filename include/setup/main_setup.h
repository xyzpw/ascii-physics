#pragma once

#include "structs/object.h"
#include "structs/world.h"
#include "enums/object_type.h"
#include "utils/arg_parser.h"

Object createObject(OBJECT_TYPE, ParsedArgs);
World createWorldWithArgs(ParsedArgs);
