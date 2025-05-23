#include <sys/ioctl.h>
#include <unistd.h>
#include <cmath>
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

bool checkPositionInsideDisplay(Position pos)
{
    struct winsize ws = getWindowSize();

    int maxCol = ws.ws_col - 1;
    int maxRow = ws.ws_row - 2;

    bool canDisplayCol = pos.column <= maxCol && pos.column >= 0;
    bool canDisplayRow = pos.row >= 0 && pos.row <= maxRow;

    return canDisplayCol && canDisplayRow;
}

Vector2D positionToVector(Position pos, double charSize)
{
    struct winsize ws = getWindowSize();

    int floorRow  = ws.ws_row - 1;

    double x = pos.column;
    double y = floorRow - pos.row;
    y -= 1.0;

    return Vector2D{x, y} * charSize;
}

Position vectorToPosition(Vector2D vector, double charSize)
{
    struct winsize ws = getWindowSize();

    int rowFloor = ws.ws_row - 1;

    vector = vector / charSize;

    int col = std::round(vector.x);
    int row = std::round(rowFloor - (vector.y + 1));

    return Position{col, row};
}
