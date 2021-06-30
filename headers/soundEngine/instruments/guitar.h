#pragma once
#include "core.h"
#include "envelope.h"
#include "logger.h"
#include "soundSynthesizer.h"
#include "sequencer.h"
#include <vector>

#include <chrono>
#include <thread>

namespace LTE
{
    namespace apoSequencerAndRythemManer
    {
        enum chords
        {
            openChord
        };

        class guitarString;

        class guitar: public sequencer
        {
            private:
                std::vector<guitarString *> *strings;
            public:
                guitar();

                virtual void PlayBeat(double currentTime, char currentBeat, note currentNote) override;
                void strummingDown(chords chord = chords::openChord );
                void strummingUp(chords chord = chords::openChord );
                
};
    }
}