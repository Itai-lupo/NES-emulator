#pragma once
#include "core.h"

namespace LaughTaleEngine
{
    class coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp ) = 0;
    };
}