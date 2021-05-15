#pragma once
#include "core.h"
#include "coreSoundWave.h"

namespace LaughTaleEngine
{
    class envelope
    {
        private:
            apoEnvelopeId id;
            double attackTime;
            double decayTime;
            double sustainAmplitude;
            double releaseTime;
            double startAmplitude;
            double masterVolume = 1.0;
            double trigerTime;
            bool isNoteOn = false;
            sondWaves mod;
            coreSoundWave *waveSampelCalculator;
            int freq;

        public:
            void setId(apoEnvelopeId id){ this->id = id; }
            apoEnvelopeId getId() { return id; }

            envelope *setAttackTime(double attackTime){ this->attackTime = attackTime; return this; }
            envelope *setDecayTime(double decayTime){ this->decayTime = decayTime; return this; }
            envelope *setSustainAmplitude(double sustainAmplitude){ this->sustainAmplitude = sustainAmplitude; return this; }
            envelope *setReleaseTime(double releaseTime){ this->releaseTime = releaseTime; return this; }
            envelope *setStartAmplitude(double startAmplitude){ this->startAmplitude = startAmplitude; return this; }
            envelope *setMasterVolume(double masterVolume){ this->masterVolume = masterVolume; return this; }
            envelope *setFrequency(int freq){ this->freq = freq; return this; }
            envelope *setSondWaveType(sondWaves mod);

            void noteOn(double time);
            void noteOff(double time);
            double getSampelAmp(double time, int16_t micAmp);
            bool isActive(double time){ return isNoteOn ||  (!isNoteOn && ( (trigerTime - time) + releaseTime > 0 )); }
    };

}
