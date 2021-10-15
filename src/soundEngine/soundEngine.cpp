#include "soundEngine.h"
#include "logger.h"
#include "app.h"
#include "soundSynthesizer.h"
#include <string>
#include <glm/glm.hpp>
#include <sys/prctl.h>


namespace LTE
{
    std::thread *soundEngine::soundThread;

    soundDevice *soundEngine::speaker;
    soundDevice *soundEngine::microphone;

    double soundEngine::time;


    void soundEngine::init()
    {
        soundSynthesizer::init();

        microphone = app::getOSAbstractFactory()->createInputSoundDevice(); 
        speaker = app::getOSAbstractFactory()->createOutputSoundDevice();

        soundThread = new std::thread(threadLoop);
    }

    void soundEngine::close()
    {
        soundSynthesizer::close();
        soundThread->join();
        delete soundThread;
        delete microphone;
        delete speaker;
    }

    double clap(double v)
    {
        return v * (v >= -1 && v <= 1) + (v > 1) - (v < -1);
    }

    void soundEngine::threadLoop()
    {
        time = 0.0;
        int16_t *buffer;

        prctl(PR_SET_NAME, "sound engine", 0, 0, 0);
        
        buffer = (int16_t *)speaker->allocate_buffer();
        while (app::keepRunning)
        {
            microphone->capture_into_buffer((char *)buffer, speaker->get_frames_per_period());

            for(int16_t *i = (int16_t*)buffer; i < buffer +  speaker->get_frames_per_period(); i++)
            {
                *i = clap(soundSynthesizer::genrateSampel(time, *i)) * (pow(2, (2 * 8) - 1) - 1);
                time += 1/(double)speaker->getSample_rate();
            }

            speaker->play_from_buffer((char *)buffer, speaker->get_frames_per_period());
        }
    }
}