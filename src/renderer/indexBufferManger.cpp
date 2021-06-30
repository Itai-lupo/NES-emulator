#include "indexBufferManger.h"
#include <algorithm>
#include "logger.h"

namespace LTE
{
    void indexBufferManger::init()
    {
        indexBuffers = new std::vector<indexBuffer *>();
    }

    void indexBufferManger::close()
    {
        std::for_each(
            indexBuffers->begin(),
            indexBuffers->end(),
            [](indexBuffer *ib){ delete ib; }
        );
        indexBuffers->clear();
        delete indexBuffers;
    }

    indexBufferId indexBufferManger::add(indexBuffer * indexBufferToAdd)
    {
        indexBufferToAdd->RendererID = indexBufferToAdd->init();
        indexBuffers->push_back(indexBufferToAdd);

        return indexBufferToAdd->RendererID;
    }

    void indexBufferManger::bind(indexBufferId id)
    {
        std::vector<indexBuffer *>::iterator ib = std::find_if(
            indexBuffers->begin(),
            indexBuffers->end(),
            [=](indexBuffer *ib) -> bool { return ib->RendererID == id; }
        );

        if(ib != indexBuffers->end())
            (*ib)->bind();
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind index buffer: no index buffer with id" << id, ib == indexBuffers->end());
    }

    void indexBufferManger::unbind(indexBufferId id)
    {
        std::vector<indexBuffer *>::iterator ib = std::find_if(
            indexBuffers->begin(),
            indexBuffers->end(),            
            [=](indexBuffer *ib) -> bool { return ib->RendererID == id; }

        );

        if(ib != indexBuffers->end())
            (*ib)->unbind();
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to unbind index buffer: no index buffer with id" << id, ib == indexBuffers->end());
    }

    uint32_t indexBufferManger::getCount(indexBufferId id)
    {

        std::vector<indexBuffer *>::iterator ib = std::find_if(
            indexBuffers->begin(),
            indexBuffers->end(),
            [=](indexBuffer *ib) -> bool { return ib->RendererID == id; }
        );

        if(ib != indexBuffers->end())
            return (*ib)->getCount();
        
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("no index buffer with id" << id, ib == indexBuffers->end());
        return 0;
    }
}
