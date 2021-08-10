#pragma once
#include "vertexArray.h"

namespace LTE
{
    class openGLVertexArray: public vertexArray
    {
        private:
            uint32_t rendererId;
        public:
            ~openGLVertexArray();
            virtual void init() override;
            virtual void AddBuffer(VertexBuffer *vb) override;
            virtual void bind() override;
            virtual void unbind() override;
    };
} 
