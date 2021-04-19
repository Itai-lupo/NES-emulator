#pragma once
#include "core.h"
#include <vector>
#include "VertexBuffer.h"

namespace LaughTaleEngine
{
    class VertexBufferManger
    {
        private:
            static std::vector<VertexBuffer *> *VertexBuffers;
        public:
            static void init();
            static void close();

            static void bind(vertexBufferId vbId);
            static void unBind(vertexBufferId vbId);

            static vertexBufferId add(VertexBuffer *vbToAdd);
            static void remove(vertexBufferId vbToAdd);

            static void changeData(vertexBufferId vbId, const void *data, uint32_t size);
            static VertexBuffer *getVB(vertexBufferId vbId);
            
            static std::vector<VertexBufferElement> getElements(vertexBufferId vbId);
            static unsigned int GetStride(vertexBufferId vbId);
            static void pushVertexBufferElement(vertexBufferId vbId, VertexBufferElement ElementToPush);
    };
}