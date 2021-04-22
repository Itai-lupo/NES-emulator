#pragma once
#include "core.h"
#include "VertexBuffer.h"

namespace LaughTaleEngine
{
    class vertexArray
    {
        public:
            vertexArrayId rendererId;
            virtual vertexArrayId init() = 0;

            virtual void AddBuffer(VertexBuffer *vb) = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;

    };
} 
