#pragma once
#include <string>
#include "core.h"

namespace LTE
{
    class texture
    {
        protected:
            const std::string& path;
            textureId id;
            uint32_t width, height;
            int channels;
        public:
            texture(const std::string& path): path(path){}
            virtual ~texture() = default;

            virtual void bind(uint32_t slot = 0) = 0;
            textureId getId(){ return id; }
    };
}