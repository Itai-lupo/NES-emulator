#pragma once
#include "renderApi.h"

namespace LTE
{
    class openGLRenderApi: public renderApi
    {
        public:
            enum class API
            {
                None = 0, OpenGL = 1
            };
        public:
            virtual void init() override;
            virtual void SetClearColor(const glm::vec4& color) override;
            virtual void Clear() override;

            virtual void DrawIndexed(uint32_t count) override;
            virtual void SetViewport(int x, int y, int width, int height) override;
    };    
}

