#pragma once

namespace LTE
{
    class openGLBase
    {
        public:
            virtual void init() = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;
    }; 
}