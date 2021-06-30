#include "soundEngine.h"
#include "logger.h"
#include "app.h"
#include "soundSynthesizer.h"
#include <string>
#include <glm/glm.hpp>

#if defined(_WIN32) || defined(_WIN64) // Windows

#elif defined(__ANDROID__)  // Android (implies Linux, so it must come first)

#elif defined(__linux__) // linux
    #include "linuxSoundApi.h"
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #if  TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1// Apple iOS
    
    #elif TARGET_OS_MAC == 1 // Apple OSX
    
    #endif
#else
    #error "platform not supported"
#endif

namespace LTE
{
    std::thread *soundEngine::soundThread;

    soundApi *soundEngine::speaker;
    soundApi *soundEngine::microphone;

    double soundEngine::time;


    void soundEngine::init()
    {
        soundSynthesizer::init();
        microphone = new linuxSoundApi("plughw:1,0,0", 44100, 1, 64, SND_PCM_FORMAT_S16_LE, SND_PCM_STREAM_CAPTURE);
        speaker = new linuxSoundApi("default", 44100, 1, 64, SND_PCM_FORMAT_S16_LE, SND_PCM_STREAM_PLAYBACK);
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
        while (app::keepRunning)
        {
            buffer = (int16_t *)speaker->allocate_buffer();
            microphone->capture_into_buffer((char *)buffer, speaker->get_frames_per_period());

            for(int16_t *i = (int16_t*)buffer; i < buffer +  speaker->get_frames_per_period(); i++)
            {
                *i = clap(soundSynthesizer::genrateSampel(time, *i)) * (pow(2, (2 * 8) - 1) - 1);
                time += 1/(double)speaker->getSample_rate();
            }

            speaker->play_from_buffer((char *)buffer, speaker->get_frames_per_period() );
        }
    }
}