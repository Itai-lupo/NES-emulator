#pragma once
#include <string>
#include "core.h"

namespace LaughTaleEngine
{
    class texture
    {
        protected:
            uint32_t id;
            uint32_t width, height;
            int channels;
        public:
            virtual ~texture() = default;

            virtual void bind(uint32_t slot = 0) = 0;
    };
}