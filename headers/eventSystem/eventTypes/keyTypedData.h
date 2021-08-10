#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct keyTypedData: public coreEventData
    {
        keyTypedData(unsigned int keycode, windowPieceId window):
            coreEventData("Key typed/"), keycode(keycode), window(window){}

        unsigned int keycode;
        windowPieceId window;

    };
}