#pragma once
#include "core.h"
#include <vector>
#include "VertexBuffer.h"

namespace LTE
{
    class VertexBufferManger
    {
        private:
            std::vector<VertexBuffer *> *VertexBuffers;
        public:
            VertexBufferManger(){ init(); }
            ~VertexBufferManger(){ close(); }

            void init();
            void close();

            void bind(vertexBufferId vbId);
            void unBind(vertexBufferId vbId);

            vertexBufferId add(VertexBuffer *vbToAdd);
            void remove(vertexBufferId vbToAdd);

            void changeData(vertexBufferId vbId, const void *data, uint32_t size);
            VertexBuffer *getVB(vertexBufferId vbId);
        
            std::vector<VertexBufferElement> getElements(vertexBufferId vbId);
            unsigned int GetStride(vertexBufferId vbId);
            void pushElement(vertexBufferId vbId, VertexBufferElement ElementToPush);
    };
}