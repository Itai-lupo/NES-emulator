#pragma once
#include "entity.h"
#include "core.h"
#include "shader.h"
#include "VertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "transform.h"
#include "asset.h"


#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include <queue>

namespace LTE
{
    class mesh: public component, public asset
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

                        meshBuilder *setVertices(float *vertexs, uint32_t size)
                        {
                            res->vertexs = vertexs;
                            res->size = size;
                            return this;
                        }


                        meshBuilder *setShaderName(const std::string& shaderName)
                        {
                            res->shaderToUse = shaderName;
                            return this;
                        }
                };
                
            private:
                entityTaleId id;

                std::string shaderToUse;

                float *vertexs;
                uint32_t size;

                uint32_t *indices; 
                uint32_t count;

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

                void setShaderName(const std::string& shaderName);

                entityTaleId getId(){ return id; }

                uint32_t getCount();
                std::string getShaderName();
                int getSize();
                float *getVB();
                float *getVertices();
                uint32_t* getIB();
                windowPieceId getWindowId(){ return winId;}

                virtual void init(gameObject *parent) override;
                virtual void end() override;
    };
}