#include "linuxSoundDevice.h"
#include "logger.h"

namespace LTE
{
    linuxSoundDevice::linuxSoundDevice(
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

    void linuxSoundDevice::OpenPCMDevicePlayback()
    {
        int err = snd_pcm_open(&handle, "default", type, 0);
        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("unable to open pcm device: " + std::string(snd_strerror(err)), err < 0);
    }


    void linuxSoundDevice::writeParamsToDriver()
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

    linuxSoundDevice::~linuxSoundDevice()
    {
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        LAUGHTALE_ENGINR_LOG_INFO("finish");
    }

    char* linuxSoundDevice::allocate_buffer() {
        unsigned int size_of_one_frame = (snd_pcm_format_width(format)/8) * channels;
        return (char*) calloc(frames_per_period, size_of_one_frame);
    }

    unsigned int linuxSoundDevice::get_frames_per_period() {
        return frames_per_period;
    }

    unsigned int linuxSoundDevice::get_bytes_per_frame() {
        unsigned int size_of_one_frame = (snd_pcm_format_width(format)/8) * channels;

        return size_of_one_frame;
    }
}