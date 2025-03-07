#include <sys/ioctl.h>
#include <unistd.h>
#include "utils/screen_utils.h"
#include "structs/position.h"

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
