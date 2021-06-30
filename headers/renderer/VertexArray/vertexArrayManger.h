#pragma once
#include "core.h"
#include "VertexBuffer.h"
#include "vertexArray.h"
#include <vector>

namespace LTE
{
    class vertexArrayManger
    {
        private:
            std::vector<vertexArray *> *vertexArrays;
        public:
            vertexArrayManger(){ init(); }
            ~vertexArrayManger(){ close(); }
            
            void init();
            void close();

            vertexArrayId add(vertexArray *va);

            void addBuffer(vertexArrayId id, VertexBuffer *vb);
            void bind(vertexArrayId id);
            void unbind(vertexArrayId id);
    };    
} 