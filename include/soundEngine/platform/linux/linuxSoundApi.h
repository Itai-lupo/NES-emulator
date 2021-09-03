#pragma once

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include "soundApi.h"

namespace LTE
{
    class linuxSoundApi: public soundApi
    {
        private:
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
            linuxSoundApi(  std::string device_name,
                            unsigned int sample_rate,
                            unsigned int channels,
                            unsigned int frames_per_period,
                            snd_pcm_format_t format,
                            enum _snd_pcm_stream type);
            ~linuxSoundApi();
            virtual char* allocate_buffer()  override;
            virtual unsigned int get_frames_per_period()  override;
            virtual unsigned int get_bytes_per_frame()  override;
            virtual long capture_into_buffer(char* buffer, snd_pcm_uframes_t frames_to_capture)  override;
            virtual unsigned int play_from_buffer(char* buffer, snd_pcm_uframes_t frames_to_play)  override;
            virtual unsigned int getSample_rate() override { return sample_rate; }
            virtual unsigned int getChannelsCount() override { return channels; };

    };       
}