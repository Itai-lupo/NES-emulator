#include "mesh.h"
#include "entity.h"
#include "core.h"
#include "openGLShader.h"
#include "openGLVertexBuffer.h"
#include "openGLVertexArray.h"
#include "openGLIndexBuffer.h"
#include "materialsManger.h"
#include "material.h"
#include "windowManger.h"

namespace LTE
{
    void mesh::setShader(const char *path)
    {
        windowManger::getWindow(winId)->operatingSystemPipLine->makeContextCurrent();

        s = new openGLShader(path);
        s->init();
    }

    void mesh::setVertexBuffer(float *vertexs, uint32_t size)
    {
        windowManger::getWindow(winId)->operatingSystemPipLine->makeContextCurrent();

        vb = new OpenGLVertexBuffer(vertexs, size);
        vb->init();
    }

    void mesh::setIndexBuffer(uint32_t *indices, uint32_t count)
    {
        windowManger::getWindow(winId)->operatingSystemPipLine->makeContextCurrent();

        ib = new LTE::openGLIndexBuffer(indices, count);
        ib->init();
    }

    void mesh::setVertexArray()
    {
        windowManger::getWindow(winId)->operatingSystemPipLine->makeContextCurrent();

        va = new openGLVertexArray();
        va->init();
        va->AddBuffer(vb);
    }

    void mesh::bind(std::vector<uint32_t> textureSlots)
    {
        setTransform(entityManger::getEntityById(parentId)->getTransform());
        va->bind();
        s->bind();
        ib->bind();
    }

    VertexBuffer *mesh::getVertexBuffer()
    {
        return vb;
    }

    shader *mesh::getShader()
    {
        return s;        
    }

    uint32_t mesh::getCount()
    {
        return ib->getCount();
    }

    void mesh::setTransform(glm::mat4 trans)
    {
        windowManger::getWindow(winId)->operatingSystemPipLine->makeContextCurrent();
        s->bind();
        s->setUniformMat4f("transform", trans);
        this->trans = trans; 
    }


    void mesh::setTransform(transform *trans)
    {
        this->trans =   glm::translate(glm::mat4(1.0f), trans->getPostion()) * 
                            glm::rotate(glm::mat4(1.0f), trans->getRotation().x, { 1.0f, 0.0f, 0.0f}) *
                            glm::rotate(glm::mat4(1.0f), trans->getRotation().y, { 0.0f, 1.0f, 0.0f}) *
                            glm::rotate(glm::mat4(1.0f), trans->getRotation().z, { 0.0f, 0.0f, 1.0f}) *
                            glm::scale(glm::mat4(1.0f), trans->getScale());

        windowManger::getWindow(winId)->operatingSystemPipLine->makeContextCurrent();
        s->bind();
        s->setUniformMat4f("transform", this->trans);
    }
}