#include <thread>
#include "soundDevice.h"
#include "core.h"
#include "envelope.h"

namespace LTE
{
    class soundEngine
    {
        private:
            static soundDevice *speaker;
            static soundDevice *microphone;
            static std::thread *soundThread;
            static double time;

            static void threadLoop();

        public:
            static void init();
            static void close();
            static double getTime(){ return time; }
    };       
}
