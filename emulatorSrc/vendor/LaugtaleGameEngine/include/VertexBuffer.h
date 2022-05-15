#pragma once
#include "core.h"
#include <vector>
#include "logger.h"

namespace LTE
{
    struct VertexBufferElement{
        unsigned int type;
        unsigned int count;
        unsigned char normalized; 
        unsigned int sizeOfType;

        VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized, unsigned int sizeOfType):
            type(type), count(count), normalized(normalized), sizeOfType(sizeOfType){}
    };

    class VertexBufferLayout
    {
        private:
            std::vector<VertexBufferElement> elements;
            unsigned int stride = 0;
        public:
            VertexBufferLayout()
                : stride(0){}

            inline VertexBufferLayout *push(const VertexBufferElement& ElementToPush)
            {
                stride += ElementToPush.sizeOfType * ElementToPush.count;
                elements.push_back(ElementToPush);
                return this;
            }

            inline const std::vector<VertexBufferElement>& getElements() { return elements; }
            inline unsigned int GetStride() { return stride; };
    };

    class VertexBuffer 
    {
        protected:
            const void *data;
            unsigned int size;
            VertexBufferLayout *VBLayout;
        public:
            VertexBuffer(const void *data, unsigned int size)
                :data(data), size(size)
                {
                    VBLayout = new VertexBufferLayout();
                }

            virtual void init() = 0;
            virtual void setData(const void *data, uint32_t size) = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;


            inline const std::vector<VertexBufferElement> getElements() { return VBLayout->getElements(); }
            inline unsigned int GetStride() { return VBLayout->GetStride(); };
            inline void pushElement(const VertexBufferElement& ElementToPush) 
            {  
                VBLayout->push(ElementToPush); 
            }
    };
}