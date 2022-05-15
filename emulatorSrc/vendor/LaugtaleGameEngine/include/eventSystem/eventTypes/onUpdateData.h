#pragma once
#include "coreEventData.h"

namespace LTE
{
    struct onUpdateData: public coreEventData
    {
        onUpdateData(uint64_t startTime, uint64_t currentTime, short DeltaTime):
            coreEventData("App update/"), startTime(startTime), currentTime(currentTime), DeltaTime(DeltaTime){}

        uint64_t startTime, currentTime;
        short DeltaTime;
    };
}