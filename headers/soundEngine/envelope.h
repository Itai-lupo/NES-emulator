#pragma once
#include "core.h"
#include "logger.h"
#include <functional>
#include <string>

namespace LaughTaleEngine
{
    class envelope
    {
        private:
            apoEnvelopeId id;
            sondWaves mod;
            std::function<double(double time, int freq, int16_t micAmp)> waveSampelCalculator;

        protected:
            double attackTime;
            double decayTime;
            double sustainAmplitude;
            double releaseTime;
            double startAmplitude = 1.0;
            double masterVolume = 1.0;
            double trigerOnTime;
            double trigerOffTime;
            bool isNoteOn = false;
            bool trigerdOnce = false;
            double freq;

            static double w(int freq);

            static double mute(double time = 0.0, double freq = 0.0, int16_t micAmp = 0.0);
            static double sin(double time, double freq, int16_t micAmp = 0.0);
            static double sowTooth(double time, double freq, int16_t micAmp = 0.0);
            static double analogSowTooth(double time, double freq, int16_t micAmp = 0.0);
            static double squre(double time, double freq, int16_t micAmp = 0.0);
            static double analogSqure(double time, double freq, int16_t micAmp = 0.0);
            static double triangels(double time, double freq, int16_t micAmp = 0.0);
            static double noise(double time = 0.0, double freq = 0.0, int16_t micAmp = 0.0);
            static double speaker(double time, double freq, int16_t micAmp);

            double MultiplayerIfNoteIsOn(double deltaTime);
            double MultiplayerIfNoteIsOff(double lifeTime, double deltaTime);


        public:
            void setId(apoEnvelopeId id){ this->id = id; }
            apoEnvelopeId getId() { return id; }

            virtual envelope *setAttackTime(double attackTime){ this->attackTime = attackTime; return this; }
            virtual envelope *setDecayTime(double decayTime){ this->decayTime = decayTime; return this; }
            virtual envelope *setSustainAmplitude(double sustainAmplitude){ this->sustainAmplitude = sustainAmplitude; return this; }
            virtual envelope *setReleaseTime(double releaseTime){ this->releaseTime = releaseTime; return this; }
            virtual envelope *setStartAmplitude(double startAmplitude){ this->startAmplitude = startAmplitude; return this; }
            virtual envelope *setMasterVolume(double masterVolume){ this->masterVolume = masterVolume; return this; }
            virtual envelope *setFrequency(double freq){ this->freq = freq; return this; }
            virtual envelope *setSondWaveType(sondWaves mod);

            virtual void noteOn(double time);
            virtual void noteOff(double time);
            virtual double getSampelAmp(double time, int16_t micAmp);
            double getEnvelopeAmpMultiplayer(double time);
            virtual bool isActive(double time)
            {
                return 
                    isNoteOn ||  
                    (!isNoteOn && ( time - trigerOffTime  < releaseTime || time - trigerOnTime < attackTime + decayTime + releaseTime )); 
            }
    };

}
