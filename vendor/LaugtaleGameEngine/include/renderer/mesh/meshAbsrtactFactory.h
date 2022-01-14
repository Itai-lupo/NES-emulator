#pragma once
#include "shader.h"
#include "vertexArray.h"
#include "VertexBuffer.h"
#include "indexBuffer.h"
#include "texture.h"

namespace LTE
{
    class meshAbsrtactFactory
    {
        public:
            virtual shader *createShader(const std::string& vertexSource, const std::string& fragmentSource) = 0;
            virtual vertexArray *createVertexArray(VertexBuffer *vb) = 0;
            virtual VertexBuffer *createVertexBuffer(float *vertexs, uint32_t size) = 0;
            virtual indexBuffer *createIndexBuffer(uint32_t *indices, uint32_t count) = 0;
            virtual texture *createTexture(const std::string& path) = 0;

            virtual void build() = 0;
    };
}