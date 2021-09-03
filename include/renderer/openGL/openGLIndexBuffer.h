#pragma once
#include "indexBuffer.h"

namespace LTE
{
    class openGLIndexBuffer : public indexBuffer
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