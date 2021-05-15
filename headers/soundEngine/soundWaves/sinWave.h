#pragma once
#include "coreSoundWave.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace LaughTaleEngine
{
    class sinWave: public coreSoundWave
    {
        public:
            virtual double calculateSampel(double time, int freq, int16_t micAmp = 0) override
            {
                return glm::sin(freq * 2.0 * glm::pi<double>() * time);
            }

    };
    
    
}