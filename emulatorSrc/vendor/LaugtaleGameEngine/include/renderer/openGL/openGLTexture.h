#pragma once
#include "openGLBase.h"
#include "texture.h"

namespace LTE
{
    class openGLTexture: public texture, public openGLBase
    {
        private:
        public:
            openGLTexture(const std::string& path);
            ~openGLTexture();
            virtual void init() override;
            virtual void bind() { bind(1); };

            virtual void bind(int slot) override;
            virtual void unbind() override;
    };
}