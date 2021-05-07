#include "soundEngine.h"
#include "logger.h"
#include "app.h"
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
    soundApi *soundEngine::soundIntrface;

    void soundEngine::init()
    {
        soundIntrface = new linuxSoundApi();
        soundThread = new std::thread(threadLoop);
    }

    void soundEngine::close()
    {
        soundThread->join();
        free(soundThread);
        free(soundIntrface);
    }      

    void soundEngine::threadLoop()
    {
        while (app::keepRunning)
        {
            uint16_t *buffer = (uint16_t *)soundIntrface->allocate_buffer();
            uint64_t time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            bool s = true;
            int j = 0;
            int f = 30;

            for(uint16_t *i = (uint16_t*)buffer; i < buffer +  soundIntrface->get_frames_per_period() / 2 ; i++)
            {
                (*i) = s * 255 * 255; //(glm::sin(110 * pow(pow(2.0, 1.0 / 12.0), 2) * 2.0 * 3.14159  * time/1000000)) * 255 * 125;
                j = (j + 1) % (f + 1);
                if(j == f)
                    s = !s;
                time += 1000000/soundIntrface->getSample_rate();
            }
            LAUGHTALE_ENGINR_LOG_INFO(time);
            soundIntrface->play_from_buffer((char *)buffer, soundIntrface->get_frames_per_period() );
        }
    }
}
