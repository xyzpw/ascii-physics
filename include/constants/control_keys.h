#pragma once

enum class CONTROL_KEY {
    KEY_QUIT = 'q',
    KEY_RESET = 'r',

    KEY_ACTION = ' ',
    KEY_MOVEUP = 'w',
    KEY_MOVEDOWN = 's',
    KEY_MOVERIGHT = 'd',
    KEY_MOVELEFT = 'a',
};

CONTROL_KEY getKeyControl(const char key)
{
    auto val = static_cast<CONTROL_KEY>(key);
    return val;
}
