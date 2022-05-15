#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct mouseClickData: public coreEventData
    {
        mouseClickData(int button, int mods, windowPieceId window):
            coreEventData(), button(button), mods(mods){ windowId = window; }

        int button, mods;
    };
}