#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class analogSqureWave: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp = 0) override
            {
                double res = 0.0;

                for (double n = 1.0; n < 40.0; n++)
                    res += glm::sin((2 * n-1) * freq * time * 2 * 3.14159) / ( 2 * n - 1);

                return res * (3.3 / 3.14159);
            }

    };
    
    
}