#pragma once

#include <ncurses.h>

enum class CONTROL_KEY {
    KEY_QUIT = 'q',
    KEY_RESET_OBJECT = 'r',

    KEY_ACTION = ' ',
    KEY_MOVEUP = 'w',
    KEY_MOVEDOWN = 's',
    KEY_MOVERIGHT = 'd',
    KEY_MOVELEFT = 'a',

    KEY_LAUNCH = 'l',

    KEY_SCALE_CONTROL_UP = static_cast<char>(KEY_UP),
    KEY_SCALE_CONTROL_DOWN = static_cast<char>(KEY_DOWN),
};

CONTROL_KEY getKeyControl(const char key)
{
    auto val = static_cast<CONTROL_KEY>(key);
    return val;
}
