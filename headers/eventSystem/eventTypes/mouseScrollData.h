#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct mouseScrollData: public coreEventData
    {
        mouseScrollData(double xOffset, double yOffset, windowPieceId window):
            coreEventData("Mouse scrolled/"), xOffset(xOffset), yOffset(yOffset), window(window){}

        double xOffset, yOffset;
        windowPieceId window;

    };
}