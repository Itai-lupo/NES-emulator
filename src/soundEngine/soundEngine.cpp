#include "soundEngine.h"
#include "logger.h"
#include "app.h"
#include "events.h"
#include <string>
#include <chrono>
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

namespace LaughTaleEngine
{
    std::thread *soundEngine::soundThread;
    soundApi *soundEngine::speaker;
    soundApi *soundEngine::microphone;
    int16_t *soundEngine::buffer;
    sondWaves soundEngine::mod;
    int soundEngine::freq;
    double soundEngine::time;

    void soundEngine::init()
    {
        microphone = new linuxSoundApi("plughw:1,0,0", 44100, 1, 64, SND_PCM_FORMAT_S16_LE, SND_PCM_STREAM_CAPTURE);
        speaker = new linuxSoundApi("default", 44100, 1, 64, SND_PCM_FORMAT_S16_LE, SND_PCM_STREAM_PLAYBACK);
        soundThread = new std::thread(threadLoop);
    }

    void soundEngine::close()
    {
        soundThread->join();
        free(soundThread);
        free(microphone);
        free(speaker);
    }      

    void soundEngine::setSoundWave(int freqInHz, sondWaves type)
    {
        freq = freqInHz;
        mod = type;
    }


    void soundEngine::threadLoop()
    {
        time = 0;
        freq = 0;
        mod = sondWaves::mute;
        int16_t amp;
        while (app::keepRunning)
        {
            buffer = (int16_t *)speaker->allocate_buffer();
            microphone->capture_into_buffer((char *)buffer, speaker->get_frames_per_period());

            for(int16_t *i = (int16_t*)buffer; i < buffer +  speaker->get_frames_per_period(); i++)
            {
                *i = genrateSampel(*i) * (pow(2, (2 * 8) - 1) - 1);
                time += 1/(double)speaker->getSample_rate();
            }

            speaker->play_from_buffer((char *)buffer, speaker->get_frames_per_period() );
        }
    }

    union funcPtrToFunc
    {
        uint64_t ptrValue;
        double (*func)(short);
    };
    

    double soundEngine::genrateSampel(int16_t micAmp)
    {
        funcPtrToFunc sondMod;
        sondMod.ptrValue = (
            ( mod == sondWaves::mute) * (uint64_t)mute + 
            ( mod == sondWaves::sin) * (uint64_t)sin + 
            ( mod == sondWaves::sowTooth) * (uint64_t)sowTooth + 
            ( mod == sondWaves::analogSowTooth) * (uint64_t)analogSowTooth + 
            ( mod == sondWaves::squre) * (uint64_t)squre + 
            ( mod == sondWaves::analogSqure) * (uint64_t)analogSqure + 
            ( mod == sondWaves::triangels) * (uint64_t)triangels + 
            ( mod == sondWaves::speaker) * (uint64_t)speakerFunc
        );

        return sondMod.func(micAmp) / 2;

    }

    double soundEngine::mute(__attribute__((unused)) int16_t micAmp)
    {
        return 0.0;
    }

    double soundEngine::sin(__attribute__((unused)) int16_t micAmp)
    {
        return glm::sin(freq * 2.0 * 3.14159 * time);
    }

    double soundEngine::sowTooth(__attribute__((unused)) int16_t micAmp)
    {
        return (2.0 / 3.14159) * (freq * 3.14159 * fmod(time, 1.0 / freq) - (3.14159 / 2.0));
    }


    double soundEngine::analogSowTooth(__attribute__((unused)) int16_t micAmp)
    {
        double res = 0.0;

		for (double n = 1.0; n < 40.0; n++)
			res += (sin(n * freq * 2 * 3.14159)) / n;

		return res * (2.0 / 3.14159);
    }

    double soundEngine::squre(__attribute__((unused)) int16_t micAmp)
    {
        return glm::sin(freq * 2.0 * 3.14159 * time) > 0 ? 1: -1;
    }

    double soundEngine::analogSqure(__attribute__((unused)) int16_t micAmp)
    {        
        double res = 0.0;

		for (double n = 1.0; n < 40.0; n++)
			res += sin((2 * n-1) * freq * 2 * 3.14159) / ( 2 * n - 1);

		return res * (4.0 / 3.14159);
    }

    double soundEngine::triangels(__attribute__((unused)) int16_t micAmp)
    {
        return asin(glm::sin(freq * 2.0 * 3.14159 * time)) * (2.0 / 3.14159);
    }

    double soundEngine::speakerFunc(int16_t micAmp)
    {
        return micAmp / (pow(2, (2 * 8) - 1) - 1);
    }

}
