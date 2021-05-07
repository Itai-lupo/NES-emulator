#include <thread>
#include <soundApi.h>



namespace LaughTaleEngine
{
    class soundEngine
    {
        private:
            static soundApi *soundIntrface;

            static void threadLoop();

            static std::thread *soundThread;
        public:
            static void init();
            static void close();
    };       
}
