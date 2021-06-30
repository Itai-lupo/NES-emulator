#pragma once
#include "vertexArray.h"

namespace LTE
{
    class openGLVertexArray: public vertexArray
    {
        ~openGLVertexArray();
        virtual vertexArrayId init() override;
        virtual void AddBuffer(VertexBuffer *vb) override;
        virtual void bind() override;
        virtual void unbind() override;
    };
} 
