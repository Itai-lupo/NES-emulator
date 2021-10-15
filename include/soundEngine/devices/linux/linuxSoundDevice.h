#pragma once
#include "core.h"
#include "soundDevice.h"

#include <string>

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

namespace LTE
{
    class linuxSoundDevice: public soundDevice
    {
        protected:
            snd_pcm_t *handle;
            // snd_pcm_hw_params_t *params;
            snd_pcm_uframes_t frames;
            unsigned int sample_rate = 44100, channels = 1;
            snd_pcm_uframes_t frames_per_period = 64;
            snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE; 
            _snd_pcm_stream type = SND_PCM_STREAM_PLAYBACK;
            std::string device_name;
            void OpenPCMDevicePlayback();
            void writeParamsToDriver();
            void printPCMInfo();
            void printHardwareParameters();
            
        public:
            linuxSoundDevice(
                std::string device_name,
                unsigned int sample_rate,
                unsigned int channels,
                unsigned int frames_per_period,
                snd_pcm_format_t format,
                _snd_pcm_stream type);
            ~linuxSoundDevice();

            virtual char* allocate_buffer() override;
            virtual unsigned int get_frames_per_period() override;
            virtual unsigned int get_bytes_per_frame() override;
            virtual unsigned int getSample_rate() override { return sample_rate; }
            virtual unsigned int getChannelsCount() override { return channels; }

    };       
}