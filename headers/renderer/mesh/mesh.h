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

namespace LTE
{
    class mesh: public component
    {
    private:
        entityTaleId id;

        shader *s;
        VertexBuffer *vb;
        indexBuffer *ib;
        vertexArray *va;

        glm::mat4     trans;
    
    public:
        bool isActive = false;
        eventLaughId onRenderId;

        mesh(){}
        mesh(LTE::windowPieceId winId){ this->winId = winId; }

        ~mesh(){}

        void setShader(const char *path);

        void setVertexBuffer(float *vertexs, uint32_t size);

        void setIndexBuffer(uint32_t *indices, uint32_t count);
        
        void setVertexArray();

        void bind(std::vector<uint32_t> textureSlots = {});

        glm::mat4 getTransform(){ return trans; }
        entityTaleId getId(){ return id; }

        void setTransform(glm::mat4 trans);
        void setTransform(transform *trans);


        VertexBuffer *getVertexBuffer();
        shader *getShader();
        uint32_t getCount();
        windowPieceId getWindowId() { return winId; }

        virtual void init(gameObject *parent) override {}
        virtual void end() override {}
    };
}