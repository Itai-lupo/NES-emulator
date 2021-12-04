#pragma once
#include "vertexArray.h"
#include "VertexBuffer.h"
#include "openGLBase.h"

namespace LTE
{
    class openGLVertexArray: public vertexArray, public openGLBase
    {
        private:
            uint32_t rendererId = 0;
            VertexBuffer *vb;
        public:
            openGLVertexArray(VertexBuffer *vb);
            ~openGLVertexArray();
            virtual void init() override;
            virtual void AddBuffer(VertexBuffer *vb) override;
            virtual void bind() override;
            virtual void unbind() override;
    };
} 
