#pragma once

namespace LaughTaleEngine
{
    class soundApi
    {
        private:
            
        public:
            virtual char* allocate_buffer() = 0;
            virtual unsigned int get_frames_per_period() = 0;
            virtual unsigned int get_bytes_per_frame() = 0;
            virtual long capture_into_buffer(char* buffer, uint64_t frames_to_capture) = 0;
            virtual unsigned int play_from_buffer(char* buffer, uint64_t frames_to_play) = 0;
            virtual unsigned int getSample_rate() = 0;
            virtual unsigned int getChannelsCount() = 0;
    };       
}