#pragma once
#include "indexBufferManger.h"

namespace LaughTaleEngine
{
    class openGLIndexBuffer : public indexBuffer
    {
        public:
            openGLIndexBuffer(unsigned int *ib, unsigned int count)
                :indexBuffer(ib, count){}
            ~openGLIndexBuffer();
            indexBufferId init();
            void bind();
            void unbind();
    };
}