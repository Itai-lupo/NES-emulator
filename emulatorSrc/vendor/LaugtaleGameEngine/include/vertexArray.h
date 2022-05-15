#pragma once
#include "core.h"
#include "VertexBuffer.h"

namespace LTE
{
    class vertexArray
    {
        public:
            virtual void init() = 0;

            virtual void AddBuffer(VertexBuffer *vb) = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;
    };
} 
