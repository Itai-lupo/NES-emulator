#include <thread>
#include <soundApi.h>



namespace LaughTaleEngine
{
    class soundEngine
    {
        private:
            static soundApi *speaker;
            static soundApi *microphone;
            static int16_t *buffer;

            static void threadLoop();

            static std::thread *soundThread;
        public:
            static void init();
            static void close();
    };       
}
