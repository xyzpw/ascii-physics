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
    init_pair(ITEM_HIGHLIGHT_YELLOW_PAIR_NUM, COLOR_YELLOW, -1);
}
