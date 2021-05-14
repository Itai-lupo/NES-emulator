#include <thread>
#include <soundApi.h>



namespace LaughTaleEngine
{
    enum sondWaves
    {
        mute,
        sin,
        sowTooth,
        analogSowTooth,
        squre,
        analogSqure,
        triangels,
        speaker
    };

    class soundEngine
    {
        private:
            static soundApi *speaker;
            static soundApi *microphone;
            static int16_t *buffer;
            static std::thread *soundThread;
            static sondWaves mod;
            static int freq;
            static double time;

            static void threadLoop();
            static double genrateSampel(int16_t micAmp);
            static double mute(int16_t micAmp);
            static double sin(int16_t micAmp);
            static double sowTooth(int16_t micAmp);
            static double analogSowTooth(int16_t micAmp);
            static double squre(int16_t micAmp);
            static double analogSqure(int16_t micAmp);
            static double triangels(int16_t micAmp);
            static double speakerFunc(int16_t micAmp);
        public:
            static void init();
            static void close();
            static void setSoundWave(int freqInHz, sondWaves type = sondWaves::mute);
    };       
}
