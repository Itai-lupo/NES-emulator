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
    void mesh::setIndexBuffer(uint32_t *indices, uint32_t count)
    {
        indices = indices;
        count = count;

        if(wasInitialized) setIndexBuffer();    
    }

    void mesh::setVertexBuffer(float *vertexs, uint32_t size)
    {
        vertexs = vertexs;
        size = size;

        if(wasInitialized) setVertexBuffer();    
    }


    void mesh::pushVertexBufferElement(const VertexBufferElement& elementToPush)
    {
        if(wasInitialized)
            vb->pushElement(elementToPush);
        else
            VBElements.push(elementToPush);
    }
    void mesh::setVertexBuffer()
    {
        window *win = windowManger::getWindow(winId);
        meshAbsrtactFactory *factory = win->context->getMeshFactory();
        vb = factory->createVertexBuffer(vertexs, size);

    }

    void mesh::setIndexBuffer()
    {
        window *win = windowManger::getWindow(winId);
        meshAbsrtactFactory *factory = win->context->getMeshFactory();
        ib = factory->createIndexBuffer(indices, count);
    }

    void mesh::setVertexArray()
    {        
        if(vb == nullptr || !wasInitialized) return;
        
        window *win = windowManger::getWindow(winId);
        meshAbsrtactFactory *factory = win->context->getMeshFactory();
        va = factory->createVertexArray(vb);

    }

    void mesh::bind(std::vector<uint32_t> textureSlots)
    {
        va->bind();
        ib->bind();
    }

    VertexBuffer *mesh::getVertexBuffer()
    {
        return vb;
    }

    uint32_t mesh::getCount()
    {
        return ib->getCount();
    }

    void mesh::init(gameObject *parent)
    {
        window *win = windowManger::getWindow(winId);
        meshAbsrtactFactory *factory = win->context->getMeshFactory();
        vb = factory->createVertexBuffer(vertexs, size);
        ib = factory->createIndexBuffer(indices, count);
        
        while (!VBElements.empty())
        {
            vb->pushElement(VBElements.front());
            VBElements.pop();
        }

        va = factory->createVertexArray(vb);
    }
}