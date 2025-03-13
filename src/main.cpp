#include <iostream>
#include <unistd.h>
#include "structs/object.h"
#include "enums/object_type.h"
#include "utils/screen_utils.h"
#include "utils/arg_parser.h"
#include "core/ncurses_loop.h"
#include "setup/input_handler.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    auto parsedArgs = parseCmdArgs(argc, argv);

    double nKg = std::stod(getArgOrPrompt(parsedArgs, "m", "mass (kg): "));

    Object mainObject(OBJECT_TYPE::OBJECT_BALL, nKg);
    mainObject.position = getCenterPosition();

    if (parsedArgs.checkKeyExists("cor")){
        double val = std::stod(parsedArgs.getKeyValue("cor"));
        mainObject.coefficientOfRestitution = val;
    }

    startNcursesLoop(mainObject);

    return 0;
}
