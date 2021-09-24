#pragma once
#include <string>
#include "core.h"
#include "logger.h"

namespace LTE
{
    class texture
    {
        protected:
            std::string path;
            textureId id;
            uint32_t width, height;
            int channels;
        public:
            texture(const std::string& path): path(path){}
            virtual ~texture() = default;

            virtual void bind() = 0;
            textureId getId(){ return id; }
    };
}