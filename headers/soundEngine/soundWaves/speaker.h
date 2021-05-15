#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class speakerMode: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp) override
            {
                return micAmp / (pow(2, (2 * 8) - 1) - 1);
            }

    };
    
    
}