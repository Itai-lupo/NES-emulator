#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shaderRenderBuffer.h"
#include "windowManger.h"
#include "shaderTypes.h"

namespace LTE
{
    shaderRenderBuffer::shaderRenderBuffer(shader *s, windowPieceId winId) : s(s), winId(winId)
    {
        meshAbsrtactFactory *meshFac = windowManger::getWindow(winId)->context->getMeshFactory();
        BatchedVertexBuffer = meshFac->createVertexBuffer({}, 0);
        BatchedIndexBuffer = meshFac->createIndexBuffer({}, 0); 

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

        glm::vec2 shapeTextureZero = { shape->getVB()[0], shape->getVB()[1] };
        glm::vec2 shapeTextureEnd = { shape->getVB()[0], shape->getVB()[1] };

        for (float *i = shape->getVB(); i < shape->getVB() + shape->getSize(); i += 3)
        {
            if(shapeTextureZero.x > i[0])
                shapeTextureZero.x = i[0];
            else if(shapeTextureEnd.x < i[0])
                shapeTextureEnd.x = i[0];

            if(shapeTextureZero.y > i[1])
                shapeTextureZero.y = i[1];
            else if(shapeTextureEnd.y < i[1])
                shapeTextureEnd.y = i[1];
        }

        glm::vec2 shapeDimensions = glm::abs(shapeTextureEnd - shapeTextureZero);
        
        glm::vec2 textureTextureZero = { m->getTexturePostionX(0), m->getTexturePostionY(0) };
        glm::vec2 textureTextureEnd =  { m->getTexturePostionX(0), m->getTexturePostionY(0) };

        for (int i = 0; i < 4; i++)
        {
            if(textureTextureZero.x > m->getTexturePostionX(i))
                textureTextureZero.x = m->getTexturePostionX(i);
            else if(textureTextureEnd.x < m->getTexturePostionX(i))
                textureTextureEnd.x = m->getTexturePostionX(i);

            if(textureTextureZero.y >  m->getTexturePostionY(i))
                textureTextureZero.y =  m->getTexturePostionY(i);
            else if(textureTextureEnd.y <  m->getTexturePostionY(i))
                textureTextureEnd.y =  m->getTexturePostionY(i);
        }

        glm::vec2 textureDimensions = glm::abs(textureTextureEnd - textureTextureZero);
        glm::vec2 rangeZero = ((shapeTextureZero) / shapeDimensions) * textureDimensions - textureTextureZero;

        for (float *i = shape->getVertices(), *j = shape->getVB(); i < shape->getVertices() + shape->getSize(); i += 3, j += 3)
        {
            temp.x = i[0];
            temp.y = i[1];
            temp.z = i[2];

            temp.textCoordX = (j[0] / shapeDimensions.x) * textureDimensions.x - rangeZero.x;
            temp.textCoordY = (j[1] / shapeDimensions.y) * textureDimensions.y - rangeZero.y;

            temp.r = m->getRGBA().r;
            temp.g = m->getRGBA().g;
            temp.b = m->getRGBA().b;
            temp.a = m->getRGBA().a;

            temp.texId = m->getTexture() ? m->getTextureId() : 0;
            verticesData.push_back(temp);
            shapeIndexCounter.push_back(0);
        }

        for (unsigned int *i = shape->getIB(); i < shape->getIB() + shape->getCount(); i++)
        {
            indices.push_back(*i + indicesOffset);
        }
        shapeIndexCounter[shapeIndexCounter.size() - 1] = shape->getCount();
    }

    void shaderRenderBuffer::setTextureIndex(std::map<textureId, int> &textures)
    {
        int usedTextureThisBatch = 0;
        while ((usedTextureThisBatch < 7 || (usedTextureThisBatch == 7 && textures[verticesData[renderBatchEnd].texId])) && renderBatchEnd != verticesData.size())
        {
            if (!textures[verticesData[renderBatchEnd].texId] && verticesData[renderBatchEnd].texId)
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
        for (vertexsData a : verticesData)
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
        for (unsigned int *a = indices.data(); a < indices.data() + indices.size(); a++)
        {
            buf += std::to_string(*a) + ", ";
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

    void shaderRenderBuffer::bind()
    {
        BatchedVertexBuffer->setData(verticesData.data(), renderBatchEnd * sizeof(vertexsData));
        BatchedIndexBuffer->setData(indices.data(), batchIndexCount);

        BatchedVertexBuffer->bind();
        BatchedVertexArray->bind();
        BatchedIndexBuffer->bind();

        verticesData.erase(verticesData.begin(), verticesData.begin() + renderBatchEnd);
        indices.erase(indices.begin(), indices.begin() + batchIndexCount);
        shapeIndexCounter.erase(shapeIndexCounter.begin(), shapeIndexCounter.begin() + renderBatchEnd);

        for (auto &indix : indices)
            indix -= renderBatchEnd;
        
        batchIndexCount = 0;
        renderBatchEnd = 0;
    }

    void shaderRenderBuffer::unbind()
    {
        BatchedVertexBuffer->unbind();
        BatchedVertexArray->unbind();
    }

    int shaderRenderBuffer::getIndecesCount()
    {
        return BatchedIndexBuffer->getCount();
    }

}
