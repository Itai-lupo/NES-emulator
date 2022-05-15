#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct KeyData: public coreEventData
    {
        KeyData(int key, int scancode, int mods, windowPieceId window):
            coreEventData(), key(key), scancode(scancode), mods(mods){ windowId = window;}

        int key, scancode, mods;
    };
}