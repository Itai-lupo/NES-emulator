#pragma once
#include "core.h"
#include "envelope.h"
#include "logger.h"

namespace LaughTaleEngine
{
    class bell : public envelope
    {
        private:
            inline static const double d12thRootOf2 = pow(2.0, 1.0 / 12.0);
        public:
            bell()
            {
                attackTime = 0.01;
                decayTime = 1.0;
                sustainAmplitude = 0.0;
                releaseTime = 0.001;
                startAmplitude = 1.0;

                masterVolume = 1/1.75;
            }

            virtual double getSampelAmp(double time, int16_t micAmp) override
            {
                if(!isActive(time)) return 0.0;

                double envelopeAmp = getEnvelopeAmpMultiplayer(time);


                envelopeAmp = ( envelopeAmp >= 0.0001) * envelopeAmp;
                
                double instrumentAmp =  
                    1.000 * glm::sin(time * w(256 * pow(d12thRootOf2, freq + 00)) + 0.0010 * w(256 * pow(d12thRootOf2, freq + 00)) * sin(time, 5)) + 
                    0.500 * glm::sin(time * w(256 * pow(d12thRootOf2, freq + 06))) +
                    0.25 * glm::sin(time * w(256 * pow(d12thRootOf2, freq + 12)));

                return envelopeAmp * instrumentAmp * masterVolume;
            }
            
    };
}