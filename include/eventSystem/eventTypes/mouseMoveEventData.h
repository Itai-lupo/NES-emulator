#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct mouseMoveData: public coreEventData
    {
        mouseMoveData(double xPos, double yPos, windowPieceId window):
            coreEventData("Mouse moved/"), xPos(xPos), yPos(yPos){ windowId = window; }

        double xPos, yPos;
    };
}