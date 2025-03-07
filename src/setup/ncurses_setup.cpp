#include <ncurses.h>
#include "setup/ncurses_setup.h"

void initializeNcurses()
{
    initscr();
    nodelay(stdscr, true);
    curs_set(0);
    noecho();
    keypad(stdscr, true);
    use_default_colors();
}
