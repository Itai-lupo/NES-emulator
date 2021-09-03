#pragma once
#include <vector>
#include "core.h"
#include<string>
#include <glm/glm.hpp>

namespace LTE
{
    class shader
    {        
        public:
            std::string filePath;
            
            shader(const std::string& filePath)
                :filePath(filePath){}

            virtual void init() = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;
            virtual void setUniform1i(const std::string& name, int value) = 0;
            virtual void setUniform1f(const std::string& name, float value) = 0;
            virtual void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;
            virtual void setUniformMat4f(const std::string& name, const glm::mat4& value) = 0;
    };
}
