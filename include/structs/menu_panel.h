#pragma once

#include <string>
#include <unordered_map>

enum class PANEL_ITEM_ACTION {
    QUIT,
};

enum class PANEL_ITEM_KEY {
    QUIT,
};

struct MenuPanelItem {
    PANEL_ITEM_ACTION action;

    std::string text;
    int row;

    MenuPanelItem(PANEL_ITEM_ACTION a, int r, std::string txt)
                : action(a), row(r), text(txt) {}
};

struct MenuPanel {
    std::unordered_map<PANEL_ITEM_KEY, MenuPanelItem> items;

    bool isOpen = false;
    int colMin, colMax; // column range of menu panel appearance
};
