#pragma once
#include "core.h"
#include "linuxSoundDevice.h"

namespace LTE
{
    class linuxOutputSoundDevice: public linuxSoundDevice
    {
        private:
            
        public:
            linuxOutputSoundDevice();
            virtual unsigned int play_from_buffer(char* buffer, uint64_t frames_to_play) override;
    };       
}