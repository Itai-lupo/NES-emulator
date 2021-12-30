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
            shapeIndexCounter.push_back(0);
        }

        for(unsigned int *i = shape->getIB(); i < shape->getIB() + shape->getCount(); i++)
        {
            indices.push_back(*i + indicesOffset);
        }
        shapeIndexCounter[shapeIndexCounter.size() - 1] = shape->getCount() ;
        
    }

    void shaderRenderBuffer::setTextureIndex(std::map<textureId, int>& textures)
    {
        int  usedTextureThisBatch = 0;
        while(( usedTextureThisBatch < 7 || (usedTextureThisBatch == 7 && textures[verticesData[renderBatchEnd].texId])) && renderBatchEnd != verticesData.size())
        {
            if(!textures[verticesData[renderBatchEnd].texId] && verticesData[renderBatchEnd].texId)
                textures[verticesData[renderBatchEnd].texId] = ++usedTextureThisBatch;

            verticesData[renderBatchEnd].textureSlot = (float)textures[verticesData[renderBatchEnd].texId];
            batchIndexCount += shapeIndexCounter[renderBatchEnd];
            renderBatchEnd++;
        }
        return;
    }

    void shaderRenderBuffer::print()
    {

        std::string buf = "\n";
        int i = 1;
        LAUGHTALE_ENGINR_LOG_INFO("print shader buffer with " << verticesData.size() << " vertices and " << indices.size() << " indices.");
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

    bool shaderRenderBuffer::isAllRendered()
    {
        return verticesData.size() == 0;
    }

    void shaderRenderBuffer::clear()
    {
        verticesData.clear();
        indices.clear();
    }



    void shaderRenderBuffer::bindRenderBatch()
    {
        BatchedVertexBuffer->setData(verticesData.data(), renderBatchEnd * sizeof(vertexsData));
        BatchedIndexBuffer->setData(indices.data(), batchIndexCount);

        BatchedVertexBuffer->bind();
        BatchedIndexBuffer->bind();
        BatchedVertexArray->bind();

        verticesData.erase(verticesData.begin(), verticesData.begin() + renderBatchEnd);
        indices.erase(indices.begin(), indices.begin() + batchIndexCount);
        shapeIndexCounter.erase(shapeIndexCounter.begin(), shapeIndexCounter.begin()  + renderBatchEnd );
        for(auto& indix: indices)
            indix -= renderBatchEnd;
        batchIndexCount = 0;
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
