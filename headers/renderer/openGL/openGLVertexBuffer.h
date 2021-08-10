#pragma once
#include "VertexBuffer.h"

namespace LTE
{

    class OpenGLVertexBuffer: public VertexBuffer
    {
        private:
            uint32_t RendererID;
        public:
            OpenGLVertexBuffer(const void *data, unsigned int size)
                :VertexBuffer(data, size){}
            ~OpenGLVertexBuffer();
            virtual void init() override;
            virtual void setData(const void *data, uint32_t size) override;
            virtual void bind() override;
            virtual void unbind() override;
    };
}