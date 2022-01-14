
#include "linuxInputSoundDevice.h"
#include "logger.h"

namespace LTE
{
    linuxInputSoundDevice::linuxInputSoundDevice(): linuxSoundDevice("default", 44100, 1, 64, SND_PCM_FORMAT_S16_LE, SND_PCM_STREAM_CAPTURE){}

    long linuxInputSoundDevice::capture_into_buffer(char* buffer, uint64_t frames_to_capture)
    {
        if(frames_to_capture != frames_per_period) 
        {
            LAUGHTALE_ENGINR_LOG_WARNING("frames_to_read must equal frames in period <" + std::to_string(frames_per_period) + ">");
            return 0;
        }

        snd_pcm_sframes_t frames_read = snd_pcm_readi(handle, buffer, frames_to_capture);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("End of file", frames_read == 0);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("error from readi: " + std::string(snd_strerror(frames_read)), frames_read < 0);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("Short read: we read <" + std::to_string(frames_read) + "> frames", frames_read != frames_per_period);
        
        return frames_read;
    }
}