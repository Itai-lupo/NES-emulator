#pragma once
#include <glm/glm.hpp>

namespace LTE
{
    class renderApi
    {
        public:
        public:
            virtual void init() = 0;
            virtual void SetClearColor(const glm::vec4& color) = 0;
            virtual void Clear() = 0;

            virtual void DrawIndexed(uint32_t count) = 0;
            virtual void SetViewport(int x, int y, int width, int height) = 0;
    };    
}

