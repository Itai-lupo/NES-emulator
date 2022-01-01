#pragma once
#include <string>
#include "core.h"
#include "asset.h"
#include "logger.h"

namespace LTE
{
    class texture : public asset
    {
        protected:
            std::string path;
            textureId id;
            int channels;

            bool isSpriteSheet;
            float spriteWidth, spriteHight;
            
            int width, height;
        public:
            texture(const std::string& path): path(path){}
            virtual ~texture() = default;

            virtual void bind(int slot) = 0;
            textureId getId(){ return id; }
    };
}