#include "soundSynthesizer.h"
#include "soundEngine.h"
#include <algorithm>
#include "logger.h"

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
        envelopes->clear();
    }

    double soundSynthesizer::genrateSampel(double time, int16_t micAmp)
    {
        double res = 0.0;
        double active = 0;
        static double lastActive = 0;
        for (uint64_t i = 0; i < envelopes->size(); i++)
        {
            double amp = (*envelopes)[i]->getSampelAmp(time, micAmp);
            active += (*envelopes)[i]->isActive(time);
            res += amp;
        }
        
        if(active > lastActive)
            active = active - lastActive/active;
        lastActive = active;
        
        return res / (active + (active == 0));
    }

    apoEnvelopeId soundSynthesizer::addEnvelope(envelope *toAdd)
    {
        toAdd->setId(nextId);
        envelopes->push_back(toAdd);
        nextId++;

        return toAdd->getId();
    }

    void soundSynthesizer::removeEnvelope(apoEnvelopeId id)
    {
        envelopes->erase(std::remove_if(
            envelopes->begin(),
            envelopes->end(),
            [=](envelope *env) -> bool { return env->getId() == id; }
        ), envelopes->end());
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