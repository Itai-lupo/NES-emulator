#include <vector>
#include<string>
#include <glm/glm.hpp>

#include "shaderRenderBuffer.h"
#include "windowManger.h"
#include "shaderTypes.h"

namespace LTE
{
    shaderRenderBuffer::shaderRenderBuffer(shader *s, windowPieceId winId): s(s), winId(winId)
    {
        meshAbsrtactFactory *meshFac =  windowManger::getWindow(winId)->context->getMeshFactory();
        BatchedIndexBuffer = meshFac->createIndexBuffer({}, 0); 
        BatchedVertexBuffer = meshFac->createVertexBuffer({}, 0);

        BatchedVertexBuffer->pushElement({LT_FLOAT, 3, false, 4});
        BatchedVertexBuffer->pushElement({LT_FLOAT, 2, false, 4});
        BatchedVertexBuffer->pushElement({LT_FLOAT, 4, false, 4});
        BatchedVertexBuffer->pushElement({LT_FLOAT, 1, false, 4});

        BatchedVertexArray = meshFac->createVertexArray(BatchedVertexBuffer);
    }

    shader *shaderRenderBuffer::getShader()
    {
        return s;
    }

    void shaderRenderBuffer::pushShape(mesh *shape, material *m)
    {
        vertexsData temp;
        unsigned int indicesOffset = verticesData.size();

        for(float *i = shape->getVB(); i < shape->getVB() + (shape->getSize() / sizeof(float)); i += 5)
        {
            temp.x = *i;
            temp.y = *(i + 1);
            temp.z = *(i + 2);
            temp.textCoordX = *(i + 3);
            temp.textCoordY = *(i + 4);


            temp.r = m->getRGBA().r;
            temp.g =  m->getRGBA().g;
            temp.b =  m->getRGBA().b;
            temp.a =  m->getRGBA().a;

            temp.texId = m->getTexture() ? m->getTextureId(): 0;
            verticesData.push_back(temp);
        }

        for(unsigned int *i = shape->getIB(); i < shape->getIB() + shape->getCount(); i++)
        {
            indices.push_back(*i + indicesOffset);
        }

        
    }

    void shaderRenderBuffer::setTextureIndex(const std::vector<std::pair<textureId, int>>& textureIndex)
    {
        for (auto& idToSlot: textureIndex)
        {
            while(idToSlot.first == verticesData[renderBatchEnd].texId)
            {
                verticesData[renderBatchEnd].textureSlot = (float)idToSlot.second;
                renderBatchEnd++;
            }
        }
        
    }

    void shaderRenderBuffer::print()
    {

        std::string buf = "\n";
        int i = 1;
        for(vertexsData a : verticesData)
        {
            buf += std::to_string(i++) + ":\t";
            buf += "x: " + std::to_string(a.x) + ",\t";
            buf += "y: " + std::to_string(a.y) + ",\t";
            buf += "z: " + std::to_string(a.z) + ",\t";

            buf += "textCoordX: " + std::to_string(a.textCoordX) + ",\t";
            buf += "textCoordY: " + std::to_string(a.textCoordY) + ",\t";

            buf += "a: " + std::to_string(a.a) + ",\t";
            buf += "b: " + std::to_string(a.b) + ",\t";
            buf += "g: " + std::to_string(a.g) + ",\t";
            buf += "r: " + std::to_string(a.r) + ",\t";

            buf += "texId: " + std::to_string(a.textureSlot) + "\n";
        }
        LAUGHTALE_ENGINR_LOG_INFO(buf);
        buf = "";
        for(unsigned int *a = indices.data(); a < indices.data() + indices.size(); a++)
        {
            buf += std::to_string(*a )+ ", ";
        }
        LAUGHTALE_ENGINR_LOG_INFO(buf);
    }

    void shaderRenderBuffer::bindRenderBatch()
    {
        BatchedVertexBuffer->setData(verticesData.data(), verticesData.size() * sizeof(vertexsData));
        BatchedIndexBuffer->setData(indices.data(), indices.size());


        BatchedIndexBuffer->bind();
        BatchedVertexArray->bind();
        verticesData.clear();
        indices.clear();
        renderBatchEnd = 0;
    }

    void shaderRenderBuffer::unbind()
    {
        BatchedIndexBuffer->unbind();
        BatchedVertexBuffer->unbind();
        BatchedVertexArray->unbind();
    }

    int shaderRenderBuffer::getVertexCount()
    {

        return BatchedIndexBuffer->getCount();
    }


}
