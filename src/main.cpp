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

    if (parsedArgs.checkKeyExistsOther("h", "help")){
        displayArgHelp();
        return 0;
    }

    World simulationWorld = createWorldWithArgs(parsedArgs);

    startNcursesLoop(simulationWorld);

    return 0;
}
