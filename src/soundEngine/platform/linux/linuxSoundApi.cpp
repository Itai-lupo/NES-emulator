
#include <glm/glm.hpp>
#include <string>
#include <algorithm>

#include "logger.h"
#include "linuxSoundApi.h"
namespace LaughTaleEngine
{
    linuxSoundApi::linuxSoundApi()
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
        snd_pcm_hw_params_set_format(handle, params, format); // SND_PCM_FORMAT_S16_LE
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

    unsigned int linuxSoundApi::capture_into_buffer(char* buffer, snd_pcm_uframes_t frames_to_capture) {
        if(frames_to_capture != frames_per_period) {
            fprintf(stderr, "frames_to_read must equal frames in period <%lu>\n", frames_per_period);
            return 0;
        }

        snd_pcm_sframes_t frames_read = snd_pcm_readi(handle, buffer, frames_to_capture);

        if(frames_read == 0) {
            fprintf(stderr, "End of file.\n");
            return 0;
        }

        if(frames_read != frames_per_period) {
            fprintf(stderr, "Short read: we read <%ld> frames\n", frames_read);
            if(frames_read < 0) {
                fprintf(stderr, "error from readi: %s\n", snd_strerror(frames_read));
                return 0;
            }
            return frames_read;
        }
    }

    unsigned int linuxSoundApi::play_from_buffer(char* buffer, snd_pcm_uframes_t frames_to_play) {
        if(frames_to_play != frames_per_period) {
            fprintf(stderr, "frames_to_play must equal frames in period <%lu>\n", frames_per_period);
            return 0;
        }

        bool done = false;
        LAUGHTALE_ENGINR_LOG_INFO("PCM handle name = " + std::string(snd_pcm_name(handle)));
        LAUGHTALE_ENGINR_LOG_INFO("frames to play = " + std::to_string(frames_to_play));
        std::string bufferStringFormat;
        for(unsigned short *i = (unsigned short *)buffer; (char *)i < buffer + 64; i++)
        {
            bufferStringFormat += std::to_string(*i) + ", ";  
        }
        
        LAUGHTALE_ENGINR_LOG_INFO("buffer = " + bufferStringFormat );
        snd_pcm_sframes_t frames_written = snd_pcm_writei(handle, buffer, 32);

        if (frames_written == -EPIPE) {
            fprintf(stderr, "underrun occurred\n");
            snd_pcm_prepare(handle);
        } else if (frames_written < 0) {
            fprintf(stderr, "error from writei: %s\n", snd_strerror(frames_written));
        }  else if (frames_written != frames_per_period) {
            fprintf(stderr, "short write, write %ld frames\n", frames_written);
        }

        return frames_written;
    }
}
