#pragma once
#include "core.h"
#include "envelope.h"
#include "logger.h"
#include "soundSynthesizer.h"
#include <vector>

#include <chrono>
#include <thread>

namespace LaughTaleEngine
{
    enum chords
    {
        openChord
    };

    class guitarString: public envelope
    {
        private:
            double StringFreq;
        public:
            guitarString(double freq)
            {
                attackTime = 0.1;
                decayTime = 0.5;
                releaseTime = 0.0001;
                startAmplitude= 1.0;
                sustainAmplitude = 0.0;
                masterVolume = 1.0/7.0;
                StringFreq = freq;
            }

            virtual double getSampelAmp(double time, int16_t micAmp)
            {
                if(!isActive(time)) return 0.0;

                double amp = getEnvelopeAmpMultiplayer(time);

                amp = ( amp > 0.0001) * amp;
                
                amp *=
                    sin(time - this->trigerOnTime , StringFreq / 8) +
                    sin(time - this->trigerOnTime , StringFreq / 4) +
                    sin(time - this->trigerOnTime , StringFreq / 2) +
                    sin(time - this->trigerOnTime , StringFreq) + 
                    sin(time - this->trigerOnTime , StringFreq * 2) +
                    sin(time - this->trigerOnTime , StringFreq * 4) +
                    sin(time - this->trigerOnTime , StringFreq * 8); 
                return amp * masterVolume;
            }

    };

    class guitar
    {
        private:
            std::vector<guitarString *> *strings;
        public:
            guitar()
            {
                strings = new std::vector<guitarString *>;
                guitarString *EString = new guitarString(082.41);
                guitarString *AString = new guitarString(110.00);
                guitarString *DString = new guitarString(146.83);
                guitarString *GString = new guitarString(196.00);
                guitarString *BString = new guitarString(246.94);
                guitarString *eString = new guitarString(329.63);

                strings->push_back(EString);
                strings->push_back(AString);
                strings->push_back(DString);
                strings->push_back(GString);
                strings->push_back(BString);
                strings->push_back(eString);

                for(envelope * string: *strings)
                {
                    soundSynthesizer::addEnvelope(string);
                }             
            }

            void strummingDown(chords chord = chords::openChord )
            {
                for(envelope * string: *strings)
                {
                    soundSynthesizer::noteOn(string->getId());
                    soundSynthesizer::noteOff(string->getId());
                    std::this_thread::sleep_for(std::chrono::milliseconds(600));
                    
                }     
            }
            
    };
}