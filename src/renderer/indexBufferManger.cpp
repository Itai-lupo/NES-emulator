#include "indexBufferManger.h"
#include <algorithm>

namespace LaughTaleEngine
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
            [](indexBuffer *ib){ free(ib); }
        );
        indexBuffers->clear();
        free(indexBuffers);
    }

    indexBufferId indexBufferManger::add(indexBuffer * indexBufferToAdd)
    {
        indexBufferToAdd->RendererID = indexBufferToAdd->init();
        indexBuffers->push_back(indexBufferToAdd);

        return indexBufferToAdd->RendererID;
    }

    void indexBufferManger::bind(indexBufferId id)
    {
        (*std::find_if(
            indexBuffers->begin(),
            indexBuffers->end(),
            [=](indexBuffer *ib) -> bool { return ib->RendererID == id; }
        ))->bind();
    }

    void indexBufferManger::unbind(indexBufferId id)
    {
        (*std::find_if(
            indexBuffers->begin(),
            indexBuffers->end(),
            [=](indexBuffer *ib) -> bool { return ib->RendererID == id; }
        ))->unbind();
    }
}
