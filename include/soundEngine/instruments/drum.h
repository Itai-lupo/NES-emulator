#pragma once
#include "core.h"
#include "envelope.h"
#include "logger.h"
#include "soundSynthesizer.h"
#include "sequencer.h"
#include <vector>

#include <chrono>
#include <thread>

namespace LTE
{
    namespace apoSequencerAndRythemManer
    {
        class drumKick: public envelope
        {
            private:
            public:
                drumKick()
                {
                    attackTime = 0.005;
                    decayTime = 0.5;
                    sustainAmplitude = 0.0;
                    releaseTime = 0.001;
                    masterVolume = 1.0;
                }

                virtual double getSampelAmp(double time, int16_t micAmp)
                {
                    if(!isActive(time)) return 0.0;

                    double amp = getEnvelopeAmpMultiplayer(time);

                    amp = ( amp > 0.0001) * amp;
                    
                    amp *=
                    0.95 * osc::sin({time - this->trigerOnTime, 25, 1.0, 1.0})
				    + 0.05 * osc::noise({}); 

                    return amp * masterVolume;
                }

        };

        class drum: public sequencer
        {
            private:
                drumKick *kick;
            public:
                drum(): sequencer(85*2, 4, 4) 
                {
                    kick = new drumKick();
                    soundSynthesizer::addEnvelope(kick);
                    addNote({1.0});
                    setSeqience("XXX..XX.X.XX..X.");
                    startSong();
                }

                virtual void PlayBeat(double currentTime, char currentBeat, note currentNote)
                {
                    if(currentBeat == 'X')
                    {
                        soundSynthesizer::noteOn(kick->getId());
                        soundSynthesizer::noteOff(kick->getId());
                    } 
                }
                
        };
    }
}