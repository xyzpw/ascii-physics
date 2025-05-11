#include <ncurses.h>
#include "setup/ncurses_setup.h"
#include "constants/object_char_constants.h"

void initializeNcurses()
{
    initscr();
    nodelay(stdscr, true);
    curs_set(0);
    noecho();
    keypad(stdscr, true);
    start_color();
    use_default_colors();
    mouseinterval(0);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    init_pair(PAIR_NUM_YELLOW_FG, COLOR_YELLOW, -1);
    init_pair(PAIR_NUM_RED_FG, COLOR_RED, -1);
    init_pair(PAIR_NUM_BRIGHT_GRAY_FG, 251, -1);
    init_pair(PAIR_NUM_DARK_GRAY_FG, 241, -1);
}
