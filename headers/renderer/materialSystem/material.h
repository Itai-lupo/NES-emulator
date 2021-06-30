#pragma once
#include "core.h"
#include "shader.h"
#include "materialsManger.h"
#include "texture.h"
#include <string>
#include "glm/glm.hpp"

namespace LTE
{
    class material
    {
        private:
            friend materialsManger;
            materialId id;

            texture *tex;
            glm::vec4 baseColor;

        public:
            material(const std::string& textureFilePath, glm::vec4 baseColor);
            material(const std::string& textureFilePath);
            material(glm::vec4 baseColor);

            ~material();
            void bind(shader *s);
            void bind(shader *s, std::vector<uint32_t> textureSlots);

            void setTexture(const std::string& path);
            void setBaseColor(glm::vec4 baseColor);
    };
}