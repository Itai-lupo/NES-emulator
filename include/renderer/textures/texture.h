#pragma once
#include <string>
#include "core.h"
#include "asset.h"
#include "logger.h"

#include "glm/glm.hpp"

namespace LTE
{
    class texture : public asset
    {
        protected:
            std::string path;
            textureId id;
            int channels;

            bool isSpriteSheet = false;
            glm::vec2 spriteDimensions;
            
            int width, height;

        public:
            texture(const std::string& path): path(path){}
            virtual ~texture() = default;

            virtual void bind(int slot) = 0;
            textureId getId(){ return id; }

            int getWidth(){return width;}
            int getHeight(){return height;}

            bool useSpriteSheet()
            {
                return isSpriteSheet;
            }

            void setSprtieData(const glm::vec2& spriteDimensions)
            {
                this->spriteDimensions = spriteDimensions;
                isSpriteSheet = true;
            }

            float getSpriteX(int spriteXId)
            {
                return spriteXId * (spriteDimensions.x / (float)width);
            }


            float getSpriteY(int spriteYId)
            {
                return spriteYId * (spriteDimensions.y / (float)height);
            }

            float getSpriteYDelte()
            {
                return spriteDimensions.y / (float)height;
            }

            float getSpriteXDelte()
            {
                return spriteDimensions.x / (float)width;
            }
    };
}