#include "core/input/keyboard_input.h"
#include "structs/object.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "constants/control_keys.h"

void handleKeyPress(const char key, Object& object)
{
    CONTROL_KEY keyControl = getKeyControl(key);

    if (keyControl == CONTROL_KEY::KEY_RESET){
        object.reset();
        return;
    }
    else if (object.isActionUsed){
        return;
    }

    int &objCol = object.position.column;
    int& objRow = object.position.row;

    auto windowSize = getWindowSize();

    auto checkColInside = [&](int col){
        return col >= 0 && col < windowSize.ws_col;
    };
    auto checkRowInside = [&](int row){
        //NOTE: The floor is 1 row before the row count, we cannot go this far.
        return row >= 0 && row < windowSize.ws_row - 1;
    };


    switch (keyControl)
    {
        case CONTROL_KEY::KEY_MOVEUP:{
            int next = object.position.row - 1;
            if (checkRowInside(objRow - 1)){
                objRow -= 1;
                object.vectors.position.y += 1;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVEDOWN:{
            if (checkRowInside(objRow + 1)){
                objRow += 1;
                object.vectors.position.y -= 1;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVERIGHT:{
            if (checkColInside(objCol + 1)){
                objCol += 1;
                object.vectors.position.x += 1;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVELEFT:{
            if (checkColInside(objCol - 1)){
                objCol -= 1;
                object.vectors.position.x -= 1;
            }
            break;
        }
        case CONTROL_KEY::KEY_ACTION:{
            object.simulate();
            break;
        }
        case CONTROL_KEY::KEY_LAUNCH:{
            auto& angle = object.launchInfo.launchAngleDeg;
            auto& velocity = object.launchInfo.velocity;

            if (angle <= 360 && angle >= 0 && velocity > 0){
                object.launch(degreesToVector(angle) * velocity);
            }

            break;
        }
    }
}
