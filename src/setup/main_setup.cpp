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

    auto setDoubleIfExists = [&](std::string key, auto setter){
        if (args.checkKeyExists(key)){
            setter(stod(args.getKeyValue(key)));
        }
    };

    setDoubleIfExists(ARG_NAME_CHAR_SIZE, [&](double val){
        world.metersPerChar = val;
        world.setWorldBounds();
    });

    setDoubleIfExists(ARG_NAME_COR, [&](double val){
        world.defaultObjectValues.coefficientOfRestitution = val;
    });

    setDoubleIfExists("g", [&](double val){
        changeGravityAcceleration(val);
    });

    setDoubleIfExists(ARG_NAME_LAUNCH_VELOCITY, [&](double val){
        world.objectInputInfo.objectLaunchVelocity = val;
    });

    setDoubleIfExists(ARG_NAME_LAUNCH_ANGLE, [&](double val){
        world.objectInputInfo.objectLaunchAngle = val;
    });

    if (args.checkKeyExists(ARG_NAME_STAT_COLLISIONS)){
        world.displayedStats.isCollisionCountVisible = true;
    }

    if (args.checkKeyExists(ARG_NAME_STAT_MAXSPEED)){
        world.displayedStats.isMaxSpeedVisible = true;
    }

    if (args.checkKeyExists(ARG_NAME_HIT_FLASH)){
        world.visualEffects.isHitFlashesVisible = true;
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
