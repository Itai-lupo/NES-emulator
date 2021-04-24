#pragma once
#include <glm/glm.hpp>
#include "indexBufferManger.h"

namespace LaughTaleEngine
{
    class renderApi
    {
        public:
        public:
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void Clear() = 0;

            virtual void DrawIndexed(uint32_t count) = 0;
    };    
}

