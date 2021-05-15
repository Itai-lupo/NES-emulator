#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class muteMode: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time = 0.0f, int freq = 0, int16_t micAmp = 0) override
            {
                return 0.0f;
            }

    };
    
    
}