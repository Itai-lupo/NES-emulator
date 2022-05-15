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
            virtual  void init() override;
            virtual  void bind() override;
            virtual  void unbind() override;

            virtual void setData(unsigned int *ib, unsigned int count) override;

    };
}