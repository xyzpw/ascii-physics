#include <sys/ioctl.h>
#include <unistd.h>
#include "utils/screen_utils.h"
#include "structs/position.h"
#include "structs/vector2d.h"

winsize getWindowSize()
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws;
}

Position getCenterPosition()
{
    struct winsize ws = getWindowSize();

    int centerCol = ws.ws_col / 2;
    int centerRow = ws.ws_row / 2;

    Position pos{.column=centerCol, .row=centerRow};
    return pos;
}

Vector2D positionToVector(Position pos)
{
    struct winsize ws = getWindowSize();

    int floorRow  = ws.ws_row - 1;

    double x = pos.column;
    double y = floorRow - pos.row;
    y -= 1.0;

    return Vector2D{x, y};
}
