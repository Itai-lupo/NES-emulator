#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class sowToothWave: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp = 0) override
            {
                return (2.0 / 3.14159) * (freq * 3.14159 * fmod(time, 1.0 / freq) - (3.14159 / 2.0));
            }

    };
    
    
}