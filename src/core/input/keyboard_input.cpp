#include <thread>
#include "core/input/keyboard_input.h"
#include "core/physics/object_physics.h"
#include "structs/object.h"
#include "structs/world.h"
#include "utils/screen_utils.h"
#include "utils/physics_utils.h"
#include "constants/control_keys.h"

void handleKeyPress(const char key, World& world)
{
    CONTROL_KEY keyControl = getKeyControl(key);

    Object& object = world.getObjectById(world.activeObjectId);
    auto& worldBounds = world.worldBounds;
    const double& metersPerChar = world.metersPerChar;

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
            if (checkRowInside(objRow - 1)){
                objRow -= 1;
                object.vectors.position.y += 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVEDOWN:{
            if (checkRowInside(objRow + 1)){
                objRow += 1;
                object.vectors.position.y -= 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVERIGHT:{
            if (checkColInside(objCol + 1)){
                objCol += 1;
                object.vectors.position.x += 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_MOVELEFT:{
            if (checkColInside(objCol - 1)){
                objCol -= 1;
                object.vectors.position.x -= 1.0 * metersPerChar;
            }
            break;
        }
        case CONTROL_KEY::KEY_ACTION:{
            std::thread th(simulateObjectMotion, std::ref(world), std::ref(object));
            th.detach();
            break;
        }
        case CONTROL_KEY::KEY_LAUNCH:{
            auto& angle = object.launchInfo.launchAngleDeg;
            auto& velocity = object.launchInfo.velocity;

            if (angle <= 360 && angle >= 0 && velocity >= 0){
                object.launch(degreesToVector(angle) * velocity);

                if (!object.isActionUsed){
                    std::thread th(simulateObjectMotion, std::ref(world), std::ref(object));
                    th.detach();
                }
            }

            break;
        }
    }
}
