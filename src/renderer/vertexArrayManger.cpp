#include "vertexArrayManger.h"
#include "algorithm"

namespace LaughTaleEngine
{
    std::vector<vertexArray *> *vertexArrayManger::vertexArrays = new std::vector<vertexArray *>();

    void vertexArrayManger::init()
    {
        vertexArrays = new std::vector<vertexArray*>();
    }

    void vertexArrayManger::close()
    {
        vertexArrays->clear();
    }    
    
    vertexArrayId vertexArrayManger::addVertxArray(vertexArray *va)
    {
        va->rendererId = va->init();
        vertexArrays->push_back(va);
        return va->rendererId;
    }    
    
    void vertexArrayManger::AddBufferToVertexArray(vertexArrayId id,  VertexBuffer& vb)
    {
        (*std::find_if(
            vertexArrays->begin(),
            vertexArrays->end(),
            [=](vertexArray *va) -> bool { return va->rendererId == id; }
        ))->AddBuffer(vb);
    }    
    
    void vertexArrayManger::bindVertexArray(vertexArrayId id)
    {
        (*std::find_if(
            vertexArrays->begin(),
            vertexArrays->end(),
            [=](vertexArray *va) -> bool { return va->rendererId == id; }
        ))->bind();
    }    
    
    void vertexArrayManger::unbindVertexArray(vertexArrayId id)
    {
        (*std::find_if(
            vertexArrays->begin(),
            vertexArrays->end(),
            [=](vertexArray *va) -> bool { return va->rendererId == id; }
        ))->unbind();
    }
}