#pragma once
#include "core.h"
#include "envelope.h"
#include <vector>

namespace LTE
{
    class soundSynthesizer
    {
        private:
            static std::vector<envelope *> *envelopes;
            static apoEnvelopeId nextId;

        public:
            static void init();
            static void close();

            static double genrateSampel(double time, short micAmp);
            static apoEnvelopeId addEnvelope(envelope *toAdd);
            static void removeEnvelope(apoEnvelopeId id);
            static void noteOn(apoEnvelopeId id);
            static void noteOff(apoEnvelopeId id);
    };
}