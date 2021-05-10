
#include <glm/glm.hpp>
#include <string>
#include <algorithm>

#include "logger.h"
#include "linuxSoundApi.h"
namespace LaughTaleEngine
{
    linuxSoundApi::linuxSoundApi(
            std::string device_name,
            unsigned int sample_rate,
            unsigned int channels,
            unsigned int frames_per_period,
            snd_pcm_format_t format,
            _snd_pcm_stream type): 
                device_name(device_name),
                sample_rate(sample_rate),
                channels(channels),
                frames_per_period(frames_per_period),
                format(format),
                type(type)
    {
        OpenPCMDevicePlayback();
        writeParamsToDriver();
    }

    void linuxSoundApi::OpenPCMDevicePlayback()
    {
       int err = snd_pcm_open(&handle, "default", type, 0);
        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("unable to open pcm device: " + std::string(snd_strerror(err)), err < 0);
    }


    void linuxSoundApi::writeParamsToDriver()
    {
        snd_pcm_hw_params_t *params;

        snd_pcm_hw_params_alloca(&params);
        snd_pcm_hw_params_any(handle, params);
        snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(handle, params, format);
        snd_pcm_hw_params_set_channels(handle, params, 1);
        int err = snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate, NULL);
        snd_pcm_hw_params_set_period_size_near(handle, params, &frames_per_period, NULL);

        err = snd_pcm_hw_params(handle, params);
        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("unable to set hw parameters: " + std::string(snd_strerror(err)), err < 0);

        snd_pcm_hw_params_get_period_size(params, &frames, NULL);
    }

    linuxSoundApi::~linuxSoundApi()
    {
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        LAUGHTALE_ENGINR_LOG_INFO("finish");
    }

    char* linuxSoundApi::allocate_buffer() {
        unsigned int size_of_one_frame = (snd_pcm_format_width(format)/8) * channels;
        return (char*) calloc(frames_per_period, size_of_one_frame);
    }

    unsigned int linuxSoundApi::get_frames_per_period() {
        return frames_per_period;
    }

    unsigned int linuxSoundApi::get_bytes_per_frame() {
        unsigned int size_of_one_frame = (snd_pcm_format_width(format)/8) * channels;

        return size_of_one_frame;
    }

    long linuxSoundApi::capture_into_buffer(char* buffer, snd_pcm_uframes_t frames_to_capture) {
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

    unsigned int linuxSoundApi::play_from_buffer(char* buffer, snd_pcm_uframes_t frames_to_play) {
        if(frames_to_play != frames_per_period) 
        {
            LAUGHTALE_ENGINR_LOG_WARNING("frames_to_play must equal frames in period <" + std::to_string(frames_per_period) + ">");
            return 0;
        }

        bool done = false;

        std::string bufferStringFormat;
        snd_pcm_sframes_t frames_written = snd_pcm_writei(handle, buffer, frames_to_play);

        if (frames_written == -EPIPE) {
            LAUGHTALE_ENGINR_LOG_ERROR("underrun occurred");
            snd_pcm_prepare(handle);
            return -EPIPE;
        }
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("error from writei: " + std::string(snd_strerror(frames_written)), frames_written < 0);        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("short write, write " + std::to_string(frames_written) + " frames", frames_written != frames_per_period);
            

        return frames_written;
    }
}
