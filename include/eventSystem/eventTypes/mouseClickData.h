#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct mouseClickData: public coreEventData
    {
        mouseClickData(int button, int mods, windowPieceId window):
            coreEventData(), button(button), mods(mods), window(window){}

        int button, mods;
        windowPieceId window;

    };
}