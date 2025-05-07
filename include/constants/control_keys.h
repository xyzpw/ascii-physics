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
    KEY_OBJECT_SELECT_NEXT = static_cast<char>(KEY_RIGHT),
    KEY_OBJECT_SELECT_PREV = static_cast<char>(KEY_LEFT),

    KEY_SELECT_PARAM_LAUNCH_VELOCITY = 'V',
    KEY_SELECT_PARAM_LAUNCH_ANGLE = 'L',
    KEY_SELECT_PARAM_MASS = 'M',

    KEY_NEW_OBJECT = 'n',
    KEY_NEW_OBSTACLE = 'N',

    KEY_CUSTOM_INPUT = 'I',
};

inline CONTROL_KEY getKeyControl(const char key)
{
    auto val = static_cast<CONTROL_KEY>(key);
    return val;
}
