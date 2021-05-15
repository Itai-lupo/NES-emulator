#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class analogSowToothWave: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp = 0) override
            {
                double res = 0.0;

                for (double n = 1.0; n < 40.0; n++)
                    res += glm::sin(n * freq * time * 2 * 3.14159) / n;

                return res * (1.7 / 3.14159);
            }

    };
    
    
}