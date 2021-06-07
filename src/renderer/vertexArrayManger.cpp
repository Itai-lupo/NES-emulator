#include "vertexArrayManger.h"
#include "VertexBufferManger.h"
#include "algorithm"
#include "logger.h"

namespace LaughTaleEngine
{
    void vertexArrayManger::init()
    {
        vertexArrays = new std::vector<vertexArray*>();
    }

    void vertexArrayManger::close()
    {
        vertexArrays->clear();
    }    
    
    vertexArrayId vertexArrayManger::add(vertexArray *va)
    {
        va->rendererId = va->init();
        vertexArrays->push_back(va);
        return va->rendererId;
    }    
    
    void vertexArrayManger::addBuffer(vertexArrayId id,  VertexBuffer *vb)
    {
        vertexArray *va = (*std::find_if(
            vertexArrays->begin(),
            vertexArrays->end(),
            [=](vertexArray *va) -> bool { return va->rendererId == id; }
        ));

        if(va != NULL)
            va->AddBuffer(vb);
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("no vertext array with id: " << id, va == NULL);

        
    }    
    
    void vertexArrayManger::bind(vertexArrayId id)
    {
        vertexArray *va = (*std::find_if(
            vertexArrays->begin(),
            vertexArrays->end(),
            [=](vertexArray *va) -> bool { return va->rendererId == id; }
        ));

        if(va != NULL)
            va->bind();
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("no vertext array with id: " << id, va == NULL);
    }    
    
    void vertexArrayManger::unbind(vertexArrayId id)
    {
        vertexArray *va = (*std::find_if(
            vertexArrays->begin(),
            vertexArrays->end(),
            [=](vertexArray *va) -> bool { return va->rendererId == id; }
        ));

        if(va != NULL)
            va->unbind();
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("no vertext array with id: " << id, va == NULL);
    }    
    
}