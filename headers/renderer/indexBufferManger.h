#pragma once
#include <vector>
#include "core.h"

namespace LaughTaleEngine
{
    class indexBuffer
    {
        protected:
            unsigned int *ib;
            unsigned int count;

        public:
            indexBuffer(unsigned int *ib, unsigned int count)
                :ib(ib), count(count){}

            indexBufferId RendererID;            

            virtual indexBufferId init() = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;

            unsigned int getCount() { return count; }
    };

    class indexBufferManger
    {
        private:
            static std::vector<indexBuffer *> *indexBuffers;
        public:
            static void init();
            static void close();

            static indexBufferId add(indexBuffer * indexBufferToAdd);
            static void bind(indexBufferId id);
            static void unbind(indexBufferId id);

    };       
}
