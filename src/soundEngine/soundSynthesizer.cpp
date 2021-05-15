#include "soundSynthesizer.h"
#include "soundEngine.h"
#include <algorithm>


namespace LaughTaleEngine
{
    std::vector<envelope *> *soundSynthesizer::envelopes;
    apoEnvelopeId soundSynthesizer::nextId;

    void soundSynthesizer::init()
    {
        envelopes = new std::vector<envelope *>();
    }

    void soundSynthesizer::close()
    {

    }

    double soundSynthesizer::genrateSampel(double time, int16_t micAmp)
    {
        double res = 0.0;
        int active = 0;
        for (envelope *i : *envelopes)
        {
            double amp = i->getSampelAmp(time, micAmp);
            active += i->isActive(time);
            res += amp;
        }
        
        return res / (active + (active == 0));
        
    }

    apoEnvelopeId soundSynthesizer::addEnvelope(envelope *toAdd)
    {
        toAdd->setId(nextId);
        envelopes->push_back(toAdd);
        nextId++;
        return toAdd->getId();
    }

    void soundSynthesizer::noteOn(apoEnvelopeId id)
    {
        double time = soundEngine::getTime();
        (*std::find_if(
            envelopes->begin(),
            envelopes->end(),
            [=](envelope *env) -> bool { return env->getId() == id; }
        ))->noteOn(time);
    }

    void soundSynthesizer::noteOff(apoEnvelopeId id)
    {
        double time = soundEngine::getTime();
        (*std::find_if(
            envelopes->begin(),
            envelopes->end(),
            [=](envelope *env) -> bool { return env->getId() == id; }
        ))->noteOff(time);
    }
}