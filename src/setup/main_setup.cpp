#include <string>
#include "setup/main_setup.h"
#include "setup/input_handler.h"
#include "structs/object.h"
#include "enums/object_type.h"
#include "utils/arg_parser.h"
#include "utils/screen_utils.h"

using std::stod;

Object createObject(OBJECT_TYPE type, ParsedArgs args)
{
    double mass = stod( getArgOrPrompt(args, "m", "mass (kg): ") );

    Object object(type, mass);

    object.position = getCenterPosition();

    if (args.checkKeyExists("cor")){
        double cor = stod( args.getKeyValue("cor") );
        object.coefficientOfRestitution = cor;
    }

    return object;
}
