#pragma once
#include "entity.h"
#include "core.h"
#include "shader.h"
#include "VertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "transform.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include <queue>

namespace LTE
{
    class mesh: public component
        {
            public: 
                class meshBuilder
                {
                    public:
                        mesh *res;
                        void reset()
                        {
                            res = new mesh();
                        }

                        meshBuilder *setIndexBuffer(uint32_t *indices, uint32_t count)
                        {
                            res->indices = indices;
                            res->count = count;
                            return this;
                        }

                        meshBuilder *setVertexBuffer(float *vertexs, uint32_t size)
                        {
                            res->vertexs = vertexs;
                            res->size = size;
                            return this;
                        }


                        meshBuilder *pushVertexBufferElement(const VertexBufferElement& elementToPush)
                        {
                            res->VBElements.push(elementToPush);
                            return this;
                        }
                };
                
            private:
                entityTaleId id;

                VertexBuffer *vb;
                indexBuffer *ib;
                vertexArray *va;

                float *vertexs;
                uint32_t size;

                uint32_t *indices; 
                uint32_t count;

                std::queue<VertexBufferElement> VBElements;
                
                bool wasInitialized = false;

                
                static inline meshBuilder *builder = nullptr;
                mesh(){}
            public:
                bool isActive = false;
                eventLaughId onRenderId;

                static mesh *build(std::function<void(meshBuilder *)> buildFunc)
                {
                    if(builder == nullptr)
                    {
                        builder = new meshBuilder();
                    }
                    builder->reset();
                    buildFunc(builder);
                    return builder->res;
                }

                ~mesh(){}

                void setIndexBuffer(uint32_t *indices, uint32_t count);
                void setVertexBuffer(float *vertexs, uint32_t size);
                void pushVertexBufferElement(const VertexBufferElement& elementToPush);

                void setVertexBuffer();
                void setIndexBuffer();
                void setVertexArray();

                void bind(std::vector<uint32_t> textureSlots = {});

                entityTaleId getId(){ return id; }

                VertexBuffer *getVertexBuffer();
                uint32_t getCount();

                virtual void init(gameObject *parent) override;
                virtual void end() override {}
    };
}