#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct keyTypedData: public coreEventData
    {
        keyTypedData(unsigned int keycode, windowPieceId window):
            coreEventData("Key typed/"), keycode(keycode){ windowId = window; }

        unsigned int keycode;
    };
}