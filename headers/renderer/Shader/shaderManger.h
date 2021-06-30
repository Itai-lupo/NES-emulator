#pragma once
#include <vector>
#include "shader.h"
#include "core.h"
#include <glm/glm.hpp>

namespace LTE
{
    class shaderManger
    {
        private:
            std::vector<shader *> *shaders;
        public:
            shaderManger(){ init(); }
            ~shaderManger(){ close(); }

            void init();
            void close();

            shaderId add(shader *shaderToAdd);
            void remove(shaderId shaderToRemove);

            void bind(shaderId id);
            void unbind(shaderId id);

            void setUniform1f(shaderId id, const std::string& name, float value);
            void setUniform1i(shaderId id, const std::string& name, int value);
            void setUniform4f(shaderId id, const std::string& name, float v0, float v1, float v2, float v3);
            void setUniformMat4f(shaderId id, const std::string& name,  const glm::mat4 &value);

            shader *getShader(shaderId id);
    };
}
