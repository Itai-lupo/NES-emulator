#include "envelope.h"
#include "logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>

namespace LaughTaleEngine
{
    double envelope::w(int freq)
    {
        return freq * 2.0 * glm::pi<double>();
    }

    double envelope::mute( __attribute__((unused)) double time, __attribute__((unused)) double freq, __attribute__((unused)) int16_t micAmp) 
    {
        return 0.0;
    }

    double envelope::sin(double time, double freq, __attribute__((unused)) int16_t micAmp)
    {
        return glm::sin( w(freq) * time);
    }

    double envelope::sowTooth(double time, double freq, __attribute__((unused)) int16_t micAmp)
    {
        return (2.0 / glm::pi<double>()) * (freq * glm::pi<double>() * fmod(time, 1.0 / freq) - (glm::pi<double>() / 2.0)); 
    }

    double envelope::analogSowTooth(double time, double freq, __attribute__((unused)) int16_t micAmp)
    {
        double res = 0.0;

        for (double n = 1.0; n < 40.0; n++)
            res += glm::sin( time * w(n  * freq))/n;

        return res * (1.7 / glm::pi<double>());
    }

    double envelope::squre(double time, double freq, __attribute__((unused)) int16_t micAmp)
    {
        return glm::sin(w(freq) * time) > 0 ? 1: -1; 
    }

    double envelope::analogSqure(double time, double freq, __attribute__((unused)) int16_t micAmp)
    {
        double res = 0.0;

        for (double n = 1.0; n < 40.0; n++)
            res += glm::sin((2 * n-1) * w(freq) * time) / ( 2 * n - 1);

        return res * (3.3 / glm::pi<double>());
    }

    double envelope::triangels(double time, double freq, __attribute__((unused)) int16_t micAmp )
    {
        return asin(glm::sin(w(freq) * time)) * (2.0 / glm::pi<double>());
    }

    double envelope::noise(__attribute__((unused)) double time , __attribute__((unused)) double freq , __attribute__((unused)) int16_t micAmp )
    {
        return 2.0 * (rand() / RAND_MAX) - 1.0;
    }

    double envelope::speaker(double time, double freq, int16_t micAmp)
    {
        return micAmp / (pow(2, (2 * 8) - 1) - 1);
    }

    envelope *envelope::setSondWaveType(sondWaves mod)
    {
        switch (mod)
        {
            case sondWaves::mute:
                waveSampelCalculator = mute;
                break;
            case sondWaves::sin:
                waveSampelCalculator = sin;
                break;
            case sondWaves::sowTooth:
                waveSampelCalculator = sowTooth;
                break;
            case sondWaves::analogSowTooth:
                waveSampelCalculator = analogSowTooth;
                break;
            case sondWaves::squre:
                waveSampelCalculator = squre;
                break;
            case sondWaves::analogSqure:
                waveSampelCalculator = analogSqure;
                break;
            case sondWaves::triangels:
                waveSampelCalculator = triangels;
                break;
            case sondWaves::noise:
                waveSampelCalculator = noise;
                break;
            case sondWaves::speaker:
                waveSampelCalculator = speaker;
                break;
            default:
                LAUGHTALE_ENGINR_LOG_FATAL("that sholdnt happen, recived value not in enum");
                break;
        }
        
        this->mod = mod;
        return this;
    }

    void envelope::noteOn(double time)
    {
        trigerdOnce = true;
        trigerOnTime = time;
        isNoteOn = true;
    }

    void envelope::noteOff(double time)
    {
        trigerOffTime = time;
        isNoteOn = false;
    }


    double envelope::getSampelAmp(double time, int16_t micAmp)
    {
        if(!isActive(time)) return 0.0;

        double amp = getEnvelopeAmpMultiplayer(time);

        amp = ( amp > 0.0001) * amp;

        return amp * waveSampelCalculator(time, freq, micAmp) * masterVolume;
    }

    double envelope::getEnvelopeAmpMultiplayer(double time)
    {
        double lifeTime = time - trigerOnTime; 
        double deltaTime = time - trigerOffTime;

        return  ((isNoteOn) * MultiplayerIfNoteIsOn(lifeTime) +
                (!isNoteOn) * MultiplayerIfNoteIsOff(lifeTime, deltaTime));
    }

    double envelope::MultiplayerIfNoteIsOn(double lifeTime)
    {
        return  (lifeTime <= attackTime) * (lifeTime / attackTime) * startAmplitude +
                (lifeTime > attackTime && lifeTime <= attackTime + decayTime) * 
                    (((lifeTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude) + 
                ( lifeTime > decayTime + attackTime) * sustainAmplitude;
            
    }

    double envelope::MultiplayerIfNoteIsOff(double lifeTime, double deltaTime)
    {
        return trigerdOnce *  
            ((lifeTime <= attackTime) * (lifeTime / attackTime) * startAmplitude + 
             (lifeTime > attackTime && lifeTime <= attackTime + decayTime) * 
                    (((lifeTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude) + 
            (lifeTime >= attackTime + decayTime) * sustainAmplitude - (deltaTime / releaseTime) * (sustainAmplitude));
    }
}