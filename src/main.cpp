#include <iostream>
#include <unistd.h>
#include "structs/object.h"
#include "enums/object_type.h"
#include "utils/screen_utils.h"
#include "core/ncurses_loop.h"

int main()
{
    setlocale(LC_ALL, "");

    double nKg;
    std::cout << "mass (kg): ";
    std::cin >> nKg;

    Object mainObject(OBJECT_TYPE::OBJECT_BALL, nKg);
    mainObject.position = getCenterPosition();

    startNcursesLoop(mainObject);

    return 0;
}
