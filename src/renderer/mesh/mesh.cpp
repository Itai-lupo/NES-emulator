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
#include "app.h"

namespace LTE
{
    void mesh::setShader(const char *path)
    {
        app::getOsAPI()->makeContextCurrent(winId);

        s = new openGLShader(path);
        s->init();
    }

    void mesh::setVertexBuffer(float *vertexs, uint32_t size)
    {
        app::getOsAPI()->makeContextCurrent(winId);

        vb = new OpenGLVertexBuffer(vertexs, size);
        vb->init();
    }

    void mesh::setIndexBuffer(uint32_t *indices, uint32_t count)
    {
        app::getOsAPI()->makeContextCurrent(winId);

        ib = new LTE::openGLIndexBuffer(indices, count);
        ib->init();
    }

    void mesh::setVertexArray()
    {        
        app::getOsAPI()->makeContextCurrent(winId);

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
        app::getOsAPI()->makeContextCurrent(winId);

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
                            
        app::getOsAPI()->makeContextCurrent(winId);
        s->bind();
        s->setUniformMat4f("transform", this->trans);
    }
}