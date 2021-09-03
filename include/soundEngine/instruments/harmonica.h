#pragma once
#include "core.h"
#include "envelope.h"

namespace LTE
{
    class harmonica : public envelope
    {
        private:
            inline static const double d12thRootOf2 = pow(2.0, 1.0 / 12.0);
        public:
            harmonica()
            {
                attackTime = 0.01;
                decayTime = 0.2;
                sustainAmplitude = 0.85;
                releaseTime = 0.1;

                masterVolume = 1/2.0;
            }

            virtual double getSampelAmp(double time, int16_t micAmp) override
            {
                if(!isActive(time)) return 0.0;

                double deltaTime = time - trigerOnTime;
                double envelopeAmp = getEnvelopeAmpMultiplayer(deltaTime);

                envelopeAmp = ( envelopeAmp >= 0.0001) * envelopeAmp;
                
                double instrumentAmp = 0.0;

                for (double n = 1.0; n < 10.0; n++)
                {
                    instrumentAmp += 
                        1.00 * glm::sin((2 * n-1) * osc::w(256 * pow(d12thRootOf2, freq * 1)) * time + 0.001 * 256 * pow(d12thRootOf2, freq * 1) * glm::sin(osc::w(4.0 ) * time) ) / ( 2 * n - 1) +
                        0.60 * glm::sin((2 * n-1) * osc::w(256 * pow(d12thRootOf2, freq * 2)) * time + 0.001 * 256 * pow(d12thRootOf2, freq * 2) * glm::sin(osc::w(8.0 ) * time) ) / ( 2 * n - 1) +
                        0.20 * glm::sin((2 * n-1) * osc::w(256 * pow(d12thRootOf2, freq * 3)) * time + 0.001 * 256 * pow(d12thRootOf2, freq * 3) * glm::sin(osc::w(16.0) * time) ) / ( 2 * n - 1);

                }
                
                instrumentAmp +=  0.05 * osc::noise({});
                return envelopeAmp * instrumentAmp * masterVolume;
            }
            
    };
}