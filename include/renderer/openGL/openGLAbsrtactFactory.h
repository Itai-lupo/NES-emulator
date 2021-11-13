#pragma once

#include <queue>
#include <atomic>

#include "meshAbsrtactFactory.h"
#include "openGLBase.h"
#include "openGLShader.h"
#include "openGLVertexArray.h"
#include "openGLVertexBuffer.h"
#include "openGLIndexBuffer.h"
#include "openGLTexture.h"

namespace LTE
{
    class openGLAbsrtactFactory: public meshAbsrtactFactory
    {
        private:
            std::queue<openGLBase*> needToInit;
        public:

            virtual shader *createShader(const std::string& vertexSource, const std::string& fragmentSource) override
            {
                openGLShader *s = new openGLShader(vertexSource, fragmentSource);
                needToInit.push(s);
                return s;
            }

            virtual vertexArray *createVertexArray(VertexBuffer *vb) override
            {
                openGLVertexArray *va = new openGLVertexArray(vb);
                needToInit.push(va);
                return va;
            }

            virtual VertexBuffer *createVertexBuffer(float *vertexs, uint32_t size) override
            {
                OpenGLVertexBuffer *vb = new OpenGLVertexBuffer(vertexs, size);
                needToInit.push(vb);
                return vb;
            }

            virtual indexBuffer *createIndexBuffer(uint32_t *indices, uint32_t count) override
            {
                openGLIndexBuffer *ib = new openGLIndexBuffer(indices, count);
                needToInit.push(ib);
                return ib;
            }

            virtual texture *createTexture(const std::string& path) override
            {
                openGLTexture *t = new openGLTexture(path);
                needToInit.push(t);
                return t;
            }

            virtual void build() override
            {
                while (!needToInit.empty())
                {
                    needToInit.front()->init();
                    needToInit.pop();
                }
                
            }

    };
}