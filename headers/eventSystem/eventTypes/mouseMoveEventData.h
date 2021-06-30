#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct mouseMoveData: public coreEventData
    {
        mouseMoveData(double xPos, double yPos, windowPieceId window):
            coreEventData(events::MouseMoved), xPos(xPos), yPos(yPos), window(window) {}

        double xPos, yPos;
        windowPieceId window;
    };
}