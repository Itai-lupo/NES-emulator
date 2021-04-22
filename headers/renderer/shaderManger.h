#pragma once
#include <vector>
#include "shader.h"
#include "core.h"

namespace LaughTaleEngine
{
    class shaderManger
    {
        private:
            static std::vector<shader *> *shaders;
        public:
            static void init();
            static void close();

            static shaderId add(shader *shaderToAdd);
            static void remove(shaderId shaderToRemove);

            static void bind(shaderId id);
            static void unbind(shaderId id);

            static void setUniform1f(shaderId id, const std::string& name, float value);
            static void setUniform1i(shaderId id, const std::string& name, int value);
            static void setUniform4f(shaderId id, const std::string& name, float v0, float v1, float v2, float v3);
            static void setUniformMat4f(shaderId id, const std::string& name,  void *value);
    };
}
