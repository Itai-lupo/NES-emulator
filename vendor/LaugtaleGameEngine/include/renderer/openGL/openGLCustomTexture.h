#pragma once
#include "openGLBase.h"
#include "texture.h"

namespace LTE
{
    class openGLCustomTexture: public texture, public openGLBase
    {
        private:
            std::pair<int, int> size;
            struct RGB
            {
                uint8_t R;
                uint8_t G;
                uint8_t B;
            };
            RGB *data;

        public:
            openGLCustomTexture(const std::pair<int, int>& size);
            ~openGLCustomTexture();
            virtual void init() override;
            virtual void bind() { bind(1); };

            virtual void bind(int slot) override;
            virtual void unbind() override;

            virtual void setRGBValue(const std::pair<int, int>& cords, const uint8_t rgb[3]) override;
    
    };
}