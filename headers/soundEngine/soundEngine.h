#include <thread>
#include "soundApi.h"
#include "core.h"
#include "envelope.h"

namespace LaughTaleEngine
{
    class soundEngine
    {
        private:
            static soundApi *speaker;
            static soundApi *microphone;
            static std::thread *soundThread;
            static double time;

            static void threadLoop();

        public:
            static void init();
            static void close();
            static double getTime(){ return time; }
    };       
}
