#include <string>
#include "structs/menu_panel.h"

void MenuPanel::addItem(PANEL_ITEM_KEY key, PANEL_ITEM_ACTION action, std::string txt)
{
    int maxRow = -1;
    for (const auto& it : this->items){
        if (it.second.row > maxRow){
            maxRow = it.second.row;
        }
    }

    this->items.insert({key, MenuPanelItem{action, maxRow + 1, txt}});
}

void MenuPanel::adjustColMin()
{
    int len = 0;

    for (const auto& it : items)
    {
        if (it.second.text.length() > len){
            len = it.second.text.length();
        }
    }

    colMin = colMax - len;
}
