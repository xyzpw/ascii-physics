#include <string>
#include "setup/main_setup.h"
#include "setup/input_handler.h"
#include "structs/object.h"
#include "structs/world.h"
#include "enums/object_type.h"
#include "utils/arg_parser.h"
#include "utils/screen_utils.h"
#include "constants/physics_constants.h"

using std::stod;

Object createObject(OBJECT_TYPE type, ParsedArgs args)
{
    double mass = stod( getArgOrPrompt(args, "m", "mass (kg): ") );

    Object object(type, mass);

    object.position = getCenterPosition();
    object.vectors.position = positionToVector(object.position);

    if (args.checkKeyExists("cor")){
        double cor = stod( args.getKeyValue("cor") );
        object.coefficientOfRestitution = cor;
    }
    if (args.checkKeyExists("g")){
        double gravity = stod( args.getKeyValue("g") );
        changeGravityAcceleration(gravity);
    }
    if (args.checkKeyExists("launch")){
        double angle = stod(
            getArgOrPrompt(args, "angle", "launch angle: ")
        );
        double velocity = stod(
            getArgOrPrompt(args, "velocity", "launch velocity: ")
        );
        object.launchInfo.launchAngleDeg = angle;
        object.launchInfo.velocity = velocity;
    }

    return object;
}
