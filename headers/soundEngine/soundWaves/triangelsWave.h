#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class triangelsWave: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp = 0) override
            {
                return asin(glm::sin(freq * 2.0 * 3.14159 * time)) * (2.0 / 3.14159);
            }

    };
    
    
}