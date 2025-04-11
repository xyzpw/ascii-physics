#include <iostream>
#include <unistd.h>
#include "structs/object.h"
#include "structs/world.h"
#include "enums/object_type.h"
#include "utils/arg_parser.h"
#include "setup/main_setup.h"
#include "core/ncurses_loop.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    auto parsedArgs = parseCmdArgs(argc, argv);

    Object mainObject = createObject(OBJECT_TYPE::OBJECT_BALL, parsedArgs);

    World simulationWorld;
    simulationWorld.objects.push_back(mainObject);
    simulationWorld.activeObjectId = mainObject.id;

    startNcursesLoop(simulationWorld);

    return 0;
}
