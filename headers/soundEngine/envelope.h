#pragma once
#include "core.h"
#include "logger.h"
#include <functional>
#include <string>

namespace LaughTaleEngine
{
    struct sondSampelData
    {
        double time;
        double freq;
        double innerFanqFreq = 0.0;
        double innerFanqAmp = 0.0;
        int16_t micAmp;
        sondSampelData(){} 

        sondSampelData(double time, double freq): 
            time(time), freq(freq) {}
            
        sondSampelData(double time, double freq, double innerFanqAmp, double innerFanqFreq): 
            time(time), freq(freq), innerFanqFreq(innerFanqFreq), innerFanqAmp(innerFanqAmp) {}


        sondSampelData(double time, double freq, int16_t micAmp): 
            time(time), freq(freq), micAmp(micAmp) {} 

        sondSampelData(int16_t micAmp): 
            micAmp(micAmp) {} 

    };

    class osc
    {
        public:
            static double w(int freq);

            static double mute(sondSampelData data);
            static double sin(sondSampelData data);
            static double sowTooth(sondSampelData data);
            static double analogSowTooth(sondSampelData data);
            static double squre(sondSampelData data);
            static double analogSqure(sondSampelData data);
            static double triangels(sondSampelData data);
            static double noise(sondSampelData data);
            static double speaker(sondSampelData data);
    };

    class envelope
    {
        private:
            apoEnvelopeId id;
            sondWaves mod;
            std::function<double(sondSampelData data)> waveSampelCalculator;

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
