#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct keyTypedData: public coreEventData
    {
        keyTypedData(unsigned int keycode, windowPieceId window):
            coreEventData(events::KeyTyped), keycode(keycode), window(window){}

        unsigned int keycode;
        windowPieceId window;

    };
}