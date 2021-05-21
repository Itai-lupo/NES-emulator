#include "envelope.h"
#include "logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>

namespace LaughTaleEngine
{
    double osc::w(int freq)
    {
        return freq * 2.0 * glm::pi<double>();
    }

    double osc::mute(__attribute__((unused)) sondSampelData data) 
    {
        return 0.0;
    }

    double osc::sin(sondSampelData data)
    {
        if(data.innerFanqAmp != 0.0)
            return glm::sin( w(data.freq) * data.time + data.innerFanqAmp * w(data.freq) * osc::sin({data.time, data.innerFanqFreq }));    
        else
            return glm::sin( w(data.freq) * data.time);
    }

    double osc::sowTooth(sondSampelData data)
    {
        return (2.0 / glm::pi<double>()) * (data.freq * glm::pi<double>() * fmod(data.time, 1.0 / data.freq) - (glm::pi<double>() / 2.0)); 
    }

    double osc::analogSowTooth(sondSampelData data)
    {
        double res = 0.0;

        for (double n = 1.0; n < 40.0; n++)
            res += glm::sin( data.time * w(n  * data.freq) + data.innerFanqAmp * w(data.freq) * sin({data.time, data.innerFanqFreq }))/n;

        return res * (1.7 / glm::pi<double>());
    }

    double osc::squre(sondSampelData data)
    {
        return glm::sin(w(data.freq) * data.time + data.innerFanqAmp * w(data.freq) * sin({data.time, data.innerFanqFreq })) > 0 ? 1: -1; 
    }

    double osc::analogSqure(sondSampelData data)
    {
        double res = 0.0;

        for (double n = 1.0; n < 40.0; n++)
            res += glm::sin((2 * n-1) * w(data.freq) * data.time + data.innerFanqAmp * w(data.freq) * sin({data.time, data.innerFanqFreq })) / ( 2 * n - 1);

        return res * (3.3 / glm::pi<double>());
    }

    double osc::triangels(sondSampelData data )
    {
        return asin(glm::sin(w(data.freq) * data.time) + data.innerFanqAmp * w(data.freq) * sin({data.time, data.innerFanqFreq })) * (2.0 / glm::pi<double>());
    }

    double osc::noise(__attribute__((unused)) sondSampelData data)
    {
        return 2.0 * (rand() / RAND_MAX) - 1.0;
    }

    double osc::speaker(sondSampelData data)
    {
        return data.micAmp / (pow(2, (2 * 8) - 1) - 1);
    }

    envelope *envelope::setSondWaveType(sondWaves mod)
    {
        switch (mod)
        {
            case sondWaves::mute:
                waveSampelCalculator = osc::mute;
                break;
            case sondWaves::sin:
                waveSampelCalculator = osc::sin;
                break;
            case sondWaves::sowTooth:
                waveSampelCalculator = osc::sowTooth;
                break;
            case sondWaves::analogSowTooth:
                waveSampelCalculator = osc::analogSowTooth;
                break;
            case sondWaves::squre:
                waveSampelCalculator = osc::squre;
                break;
            case sondWaves::analogSqure:
                waveSampelCalculator = osc::analogSqure;
                break;
            case sondWaves::triangels:
                waveSampelCalculator = osc::triangels;
                break;
            case sondWaves::noise:
                waveSampelCalculator = osc::noise;
                break;
            case sondWaves::speaker:
                waveSampelCalculator = osc::speaker;
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
                LAUGHTALE_ENGINR_LOG_INFO("A");

        sondSampelData sampelData(time, freq, micAmp);
        return amp * waveSampelCalculator(sampelData) * masterVolume;
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