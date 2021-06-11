#pragma once
#include "core.h"
#include "material.h"
#include "texture.h"
#include <string.h>

namespace LaughTaleEngine
{
    class texture2DMatrial: public material
    {
        private:
            texture *tex;
        public:
            uint32_t slot = 0;
            texture2DMatrial(const std::string& textureFilePath);
            ~texture2DMatrial();
            virtual void bind(shader *s) override;
    };
}