#include "sequencer.h"
#include "soundEngine.h"
#include <chrono>
#include <sys/prctl.h>

#include "logger.h"
namespace LTE
{
    sequencer::sequencer( double bpm, int beats, int subBeats ): bpm(bpm), beats(beats), subBeats(subBeats), totalBeats(beats * subBeats), BeatTime(1.0/bpm)
    {

    }

    void sequencer::song()
    {
        std::string songName = "song" + sequence;
        prctl(PR_SET_NAME, songName.c_str(), 0, 0, 0);

        int currentBeatIndex = 0;
        int currentNoteIndex = 0;
        int i = 0;
        double noteStartTime = 0.0;
        double time;
        while (songOn)
        {
            time = soundEngine::getTime();
            PlayBeat(time, sequence[currentBeatIndex], notes[currentNoteIndex]);
            currentBeatIndex = (currentBeatIndex + 1) % totalBeats;
            currentNoteIndex = (currentNoteIndex + 1) % notes.size();
            std::this_thread::sleep_for(std::chrono::nanoseconds((long)(BeatTime*60000000000)));
        }
        
    } 

    void sequencer::startSong()
    {
        songOn = true;
        songThread = new std::thread(&sequencer::song, this);
    }

    void sequencer::stopSong()
    {
        songOn = false;
        songThread->join();
        delete songThread;
    }
}