#include "mesh.h"
#include "entity.h"
#include "core.h"
#include "openGLShader.h"
#include "openGLVertexBuffer.h"
#include "openGLVertexArray.h"
#include "openGLIndexBuffer.h"
#include "materialsManger.h"
#include "material.h"
#include "window.h"

namespace LaughTaleEngine
{
    mesh::mesh(windowPieceId windowId): IEntity(), windowId(windowId){}


    void mesh::setShader(const char *path)
    {
        shader *s = new openGLShader(path);
        ShaderId = windowManger::add(windowId, s);
    }

    void mesh::setShader(shaderId id)
    {
        ShaderId = id;
    }

    void mesh::setVertexBuffer(float *vertexs, uint32_t size)
    {
        VertexBuffer *VB = new OpenGLVertexBuffer(vertexs, size);
        VBId = LaughTaleEngine::windowManger::add(windowId, VB);
    }

    void mesh::setVertexBuffer(vertexBufferId id)
    {
        VBId = id;
    }

    void mesh::setIndexBuffer(uint32_t *indices, uint32_t count)
    {
        LaughTaleEngine::indexBuffer *IB = new LaughTaleEngine::openGLIndexBuffer(indices, count);
        IBId = LaughTaleEngine::windowManger::add(windowId, IB);
    }

    void mesh::setIndexBuffer(indexBufferId id)
    {
        IBId = id;
    }


    void mesh::setVertexArray()
    {
        vertexArray *VA = new openGLVertexArray();
        VAId = windowManger::add(windowId, VA);
        
        windowManger::addBuffer(windowId, VAId, VBId);
    }

    void mesh::setVertexArray(vertexArrayId id)
    {
        VAId = id;
    }

    void mesh::bind(std::vector<uint32_t> textureSlots)
    {
        windowManger::bindVA(windowId, VAId);
        windowManger::bindS(windowId, ShaderId);
        windowManger::bindIB(windowId, IBId);
        materialsManger::bind(mateId, getShader(), textureSlots);
    }

    VertexBuffer *mesh::getVertexBuffer()
    {
        return  windowManger::getVertexBufferManger(windowId)->getVB(VBId);
    }

    shader *mesh::getShader()
    {
        
        shaderManger *m =  windowManger::getShaderManger(windowId);
        if(m != nullptr)
            return  m->getShader(ShaderId);
        LAUGHTALE_ENGINR_LOG_ERROR("coulden get mesh shader");
        return nullptr;        
    }

    uint32_t mesh::getCount()
    {
        return  windowManger::getIndexBufferCount(windowId, IBId);
    }

    void mesh::setTransform(glm::mat4 transform)
    {
        windowManger::bindContext(windowId);
        windowManger::bindS(windowId, ShaderId);
        shader *s = getShader();
        if(s != nullptr) 
            s->setUniformMat4f("transform", transform);
        this->transform = transform; 
    }
    
    void mesh::setMaterial(const std::string& path, glm::vec4 color)
    {
        mateId = materialsManger::addMatrial(new material(path, color));
        windowManger::bindContext(windowId);

        materialsManger::bind(mateId, getShader());
    }

    void mesh::setMaterial(materialId material)
    {
        mateId = material;
        windowManger::bindContext(windowId);

        materialsManger::bind(mateId, getShader());
    }
    

}