#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct WindowResizeData: public coreEventData
    {
        WindowResizeData(int width, int height, windowPieceId window):
            coreEventData("Window resize/"), windowWidth(width), windowHeight(height){ windowId = window; }

        int windowWidth, windowHeight;
    };
}