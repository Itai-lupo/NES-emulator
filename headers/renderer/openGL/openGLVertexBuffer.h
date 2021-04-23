#pragma once
#include "VertexBuffer.h"

namespace LaughTaleEngine
{

    class OpenGLVertexBuffer: public VertexBuffer
    {
        public:
            OpenGLVertexBuffer(const void *data, unsigned int size)
                :VertexBuffer(data, size){}
            ~OpenGLVertexBuffer();
            virtual vertexBufferId init() override;
            virtual void setData(const void *data, uint32_t size) override;
            virtual void bind() override;
            virtual void unbind() override;
    };
}