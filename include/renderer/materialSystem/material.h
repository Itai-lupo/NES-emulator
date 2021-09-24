#pragma once
#include "core.h"
#include "shader.h"
#include "materialsManger.h"
#include "texture.h"
#include "component.h"
#include <string>
#include "glm/glm.hpp"

namespace LTE
{
    class material: public component
    {
        private:
            friend materialsManger;
            materialId id;

            texture *tex;
            glm::vec4 baseColor;

            std::string texturePath;

        public:
            material(const std::string& textureFilePath, glm::vec4 baseColor);
            material(const std::string& textureFilePath);
            material(glm::vec4 baseColor);

            void bind(shader *s);
            void bind(shader *s, std::vector<uint32_t> textureSlots);

            void setTexture(texture *tex);
            void setTexture(const std::string& path);
            void setBaseColor(glm::vec4 baseColor);
            
            glm::vec4 getRGBA();


            virtual void init(gameObject *parent) override;
            virtual void end() override;
    };
}