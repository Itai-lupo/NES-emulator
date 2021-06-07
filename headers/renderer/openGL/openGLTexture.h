#pragma once
#include "texture.h"

namespace LaughTaleEngine
{
    class openGLTexture: public texture
    {
        private:
        public:
            openGLTexture(const std::string& path);

            virtual void bind(uint32_t slot = 0) override;
    };
}