#pragma once
#include "core.h"
#include "linuxOutputSoundDevice.h"
#include "logger.h"

namespace LTE
{

    linuxOutputSoundDevice::linuxOutputSoundDevice(): linuxSoundDevice("default", 44100, 1, 64, SND_PCM_FORMAT_S16_LE, SND_PCM_STREAM_PLAYBACK){}


    unsigned int linuxOutputSoundDevice::play_from_buffer(char* buffer, uint64_t frames_to_play)
    {
        if(frames_to_play != frames_per_period) 
        {
            LAUGHTALE_ENGINR_LOG_WARNING("frames_to_play must equal frames in period <" + std::to_string(frames_per_period) + ">");
            return 0;
        }

        bool done = false;

        std::string bufferStringFormat;
        snd_pcm_sframes_t frames_written = snd_pcm_writei(handle, buffer, frames_to_play);

        if (frames_written == -EPIPE) {
            snd_pcm_recover(handle, -EPIPE, 1);
            snd_pcm_prepare(handle);
            return -EPIPE;
        }
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("error from writei: " + std::string(snd_strerror(frames_written)), frames_written < 0);        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("short write, write " + std::to_string(frames_written) + " frames", frames_written != frames_per_period);
            

        return frames_written;
    }
}