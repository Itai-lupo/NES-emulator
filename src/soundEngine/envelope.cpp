#include "envelope.h"
#include "coreSoundWave.h"
#include "logger.h"
#include "mute.h"
#include "sinWave.h"
#include "sowToothWave.h"
#include "analogSowToothWave.h"
#include "squreWave.h"
#include "analogSqureWave.h"
#include "triangelsWave.h"
#include "speaker.h"

namespace LaughTaleEngine
{
    envelope *envelope::setSondWaveType(sondWaves mod)
    {
        switch (mod)
        {
            case sondWaves::mute:
                waveSampelCalculator = new muteMode();
                break;
            case sondWaves::sin:
                waveSampelCalculator = new sinWave();
                break;
            case sondWaves::sowTooth:
                waveSampelCalculator = new sowToothWave();
                break;
            case sondWaves::analogSowTooth:
                waveSampelCalculator = new analogSowToothWave();
                break;
            case sondWaves::squre:
                waveSampelCalculator = new squreWave();
                break;
            case sondWaves::analogSqure:
                waveSampelCalculator = new analogSqureWave();
                break;
            case sondWaves::triangels:
                waveSampelCalculator = new triangelsWave();
                break;
            case sondWaves::speaker:
                waveSampelCalculator = new speakerMode();
                break;
            default:
                break;
        }
        this->mod = mod;
        return this;
    }

    void envelope::noteOn(double time)
    {
        trigerTime = time;
        isNoteOn = true;
    }

    void envelope::noteOff(double time)
    {
        trigerTime = time;
        isNoteOn = false;
    }

    double envelope::getSampelAmp(double time, int16_t micAmp)
    {
        double deltaTime = time - trigerTime;
        double amp = 
            (isNoteOn) * (
                (deltaTime <= attackTime) * (deltaTime / attackTime) * startAmplitude + 
                (deltaTime > attackTime && deltaTime <= attackTime + decayTime) * 
                    (((deltaTime - attackTime) / decayTime) * (sustainAmplitude - startAmplitude) + startAmplitude) + 
                ( deltaTime > decayTime + attackTime) * sustainAmplitude
            ) +
            (!isNoteOn) * ( sustainAmplitude - (deltaTime / releaseTime) * (sustainAmplitude));

        amp = ( amp >= 0.0001) * amp;

        return amp * waveSampelCalculator->calculateSampel(time, freq, micAmp) * masterVolume;
    }
}