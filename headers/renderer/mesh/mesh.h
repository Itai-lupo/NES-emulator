#pragma once
#include "entity.h"
#include "core.h"
#include "shader.h"
#include "VertexBuffer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace LaughTaleEngine
{
    class mesh: public IEntity
    {
    private:
        windowPieceId windowId;

        vertexBufferId VBId;
        indexBufferId IBId;
        vertexArrayId VAId;
        shaderId      ShaderId;
        materialId    mateId;

        glm::mat4     transform;
    
    public:
        bool isActive = false;
        eventLaughId onRenderId;

        mesh(windowPieceId windowId);
        ~mesh(){LAUGHTALE_ENGINR_LOG_INFO("destroy mesh with id: " << getId());}

        void setShader(const char *path);
        void setShader(shaderId id);

        void setVertexBuffer(float *vertexs, uint32_t size);
        void setVertexBuffer(vertexBufferId id);

        void setIndexBuffer(uint32_t *indices, uint32_t count);
        void setIndexBuffer(indexBufferId id);

        void setVertexArray();
        void setVertexArray(vertexArrayId id);

        void bind(std::vector<uint32_t> textureSlots = {});

        glm::mat4 getTransform(){ return transform; }
        materialId  getMaterialId(){ return mateId; }

        void setTransform(glm::mat4 transform);

        void setMaterial(const std::string& path, glm::vec4 color);
        void setMaterial(materialId material);


        VertexBuffer *getVertexBuffer();
        shader *getShader();
        uint32_t getCount();
        windowPieceId getWindowId() { return windowId; }
    };
}