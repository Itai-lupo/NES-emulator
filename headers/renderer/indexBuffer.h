#pragma once
#include <vector>
#include "core.h"

namespace LTE
{
    class indexBuffer
    {
        protected:
            unsigned int *ib;
            unsigned int count;

        public:
            indexBuffer(unsigned int *ib, unsigned int count)
                :ib(ib), count(count){}
        

            virtual void init() = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;

            unsigned int getCount() { return count; }
    };
}
