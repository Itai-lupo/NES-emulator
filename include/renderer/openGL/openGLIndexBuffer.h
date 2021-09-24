#pragma once
#include "openGLBase.h"
#include "indexBuffer.h"

namespace LTE
{
    class openGLIndexBuffer : public indexBuffer, public openGLBase
    {
        private:
            uint32_t RendererID;
        public:
            openGLIndexBuffer(unsigned int *ib, unsigned int count)
                :indexBuffer(ib, count){}
            ~openGLIndexBuffer();
            void init();
            void bind();
            void unbind();
    };
}