#pragma once
#include "core.h"
#include "envelope.h"
#include "logger.h"
#include "sequencer.h"

namespace LaughTaleEngine
{
    namespace apoSequencerAndRythemManer
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
                        1.000 * glm::sin(time * osc::w(256 * pow(d12thRootOf2, freq + 00)) + 0.0010 * osc::w(256 * pow(d12thRootOf2, freq + 00)) * osc::sin({time, 5})) + 
                        0.500 * glm::sin(time * osc::w(256 * pow(d12thRootOf2, freq + 06))) +
                        0.25 * glm::sin(time * osc::w(256 * pow(d12thRootOf2, freq + 12)));

                    return envelopeAmp * instrumentAmp * masterVolume;
                }
                
        };

        class bellRing: public sequencer
        {
            private:
                bell *b;
            public:
                bellRing(): sequencer(85, 4, 4) 
                {
                    b = new bell();
                    soundSynthesizer::addEnvelope(b);
                    addNote({1.0});
                    setSeqience("X...X.X.XX.X.XXX");
                    startSong();
                }

                virtual void PlayBeat(double currentTime, char currentBeat, note currentNote)
                {
                    if(currentBeat == 'X')
                    {
                        soundSynthesizer::noteOn(b->getId());
                        soundSynthesizer::noteOff(b->getId());
                    } 
                }
                
        };
    }
}