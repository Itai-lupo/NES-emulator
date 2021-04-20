#pragma once
#include "core.h"
#include "VertexBuffer.h"
#include "vertexArray.h"
#include <vector>

namespace LaughTaleEngine
{
    class vertexArrayManger
    {
        private:
            static std::vector<vertexArray *> *vertexArrays;
        public:
            static void init();
            static void close();

            static vertexArrayId addVertxArray(vertexArray *va);

            static void AddBufferToVertexArray(vertexArrayId id, VertexBuffer& vb);
            static void bindVertexArray(vertexArrayId id);
            static void unbindVertexArray(vertexArrayId id);
    };    
} 