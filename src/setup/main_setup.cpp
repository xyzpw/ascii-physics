#include <string>
#include <iostream>
#include "setup/main_setup.h"
#include "setup/input_handler.h"
#include "structs/object.h"
#include "structs/world.h"
#include "enums/object_type.h"
#include "utils/arg_parser.h"
#include "utils/screen_utils.h"
#include "constants/physics_constants.h"
#include "constants/arg_name_constants.h"

using std::stod;

World createWorldWithArgs(ParsedArgs args)
{
    World world;

    double mass = stod(getArgOrPrompt(args, "m", "mass (kg): "));

    world.defaultObjectValues.objectMass = mass;

    if (args.checkKeyExists(ARG_NAME_CHAR_SIZE)){
        double scale = stod(args.getKeyValue(ARG_NAME_CHAR_SIZE));
        world.metersPerChar = scale;
        world.setWorldBounds();
    }
    if (args.checkKeyExists(ARG_NAME_COR)){
        double cor = stod(args.getKeyValue(ARG_NAME_COR));
        world.defaultObjectValues.coefficientOfRestitution = cor;
    }
    if (args.checkKeyExists("g")){
        double gravity = stod(args.getKeyValue("g"));
        changeGravityAcceleration(gravity);
    }
    if (args.checkKeyExists(ARG_NAME_LAUNCH_VELOCITY)){
        double velocity = stod(args.getKeyValue(ARG_NAME_LAUNCH_VELOCITY));
        world.objectInputInfo.objectLaunchVelocity = velocity;
    }
    if (args.checkKeyExists(ARG_NAME_LAUNCH_ANGLE)){
        double angle = stod(args.getKeyValue(ARG_NAME_LAUNCH_ANGLE));
        world.objectInputInfo.objectLaunchAngle = angle;
    }
    if (args.checkKeyExists(ARG_NAME_STAT_COLLISIONS)){
        world.displayedStats.isCollisionCountVisible = true;
    }

    world.addObject(OBJECT_TYPE::OBJECT_BALL, mass);
    world.activeObjectId = world.objects.at(0).id;

    return world;
}

void displayArgHelp()
{
    const int spaces = 32;

    std::cout << "options:\n\n";

    for (auto it : helpMenu){
        std::string line = "  " + it.first;
        line += std::string(spaces - line.length(), ' ');
        line += it.second;
        std::cout << line << std::endl;
    }
}
