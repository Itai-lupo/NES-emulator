#include "window.h"
#include "entity.h"
#include "logger.h"
#include <string>
#include <functional>
#include <algorithm>

#define findWinById(windowId)                                           \
    (*std::find_if(                                                     \
            windows.begin(),                                            \
            windows.end(),                                              \
            [=](window *win)-> bool { return win->id == windowId; }     \
        ))

class windowEntity: public LaughTaleEngine::IEntity
{
    public:
        windowEntity(window *win): win(win) {}
        window *win;
};

namespace LaughTaleEngine
{
    bool windowManger::VSync = false;
    static std::vector<window*> windows = std::vector<window*>();

    void windowManger::setVSync(bool enabled){ window::setVSync(VSync = enabled); }

    void windowManger::init(){
        eventManger::addEvent(events::AppUpdate, onUpdate, -1);

        setVSync(true);
    }
    
    void windowManger::close(){}    

    void onWindowResize(IEntity *eventEntity, IEventData *sendor)
    {
        WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
        windowEntity* entityData = static_cast<windowEntity*>(eventEntity);

        entityData->win->Width = sendorData->windowWidth;
        entityData->win->Height = sendorData->windowHeight;
    }

    void onWindowClose(IEntity *eventEntity, IEventData *sendor)
    {
        findWinById(sendor->windowId)->Shutdown(windowManger::getWindow(sendor->windowId));

        std::remove_if(
            windows.begin(), 
            windows.end(), 
            [=](window *win)-> bool { return win->id == sendor->windowId; }
        );
    }

    windowPieceId windowManger::addWindow(const std::string& title, bool useImGui, unsigned int width, unsigned int height, renderAPI renderAPIType)
    {
        window *newWin = new window(title, width, height, useImGui, renderAPIType);
        window::Init(newWin);
        windows.push_back(newWin);

        windowEntity *windowEntityData = new windowEntity(newWin);
        entityTaleId windowEntityId = entityManger::addEntity(dynamic_cast<windowEntity*>(windowEntityData));
        eventManger::addEvent(events::WindowResize, onWindowResize, windowEntityId, newWin->id);
        // eventManger::addEvent(events::WindowClose, onWindowClose, -1, newWin->id);
        
        return newWin->id;
    }

    void windowManger::onUpdate(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        for(window *win: windows)
        {
            win->onUpdate(win, sendor);
        }
    }

    windowPtr windowManger::raftelIdToWindowReference(windowPieceId windowId)
    {
        return (GLFWwindow*)windowId;
    }

    window *windowManger::getWindow(windowPieceId windowId)
    {
        return findWinById(windowId);
    }

    unsigned int windowManger::getWidth(windowPieceId windowId)
    { 
        return findWinById(windowId)->Width;
    }
    unsigned int windowManger::getHeight(windowPieceId windowId)
    { 
        return findWinById(windowId)->Height;
    }

    void windowManger::bindContext(windowPieceId windowId)
    {
        findWinById(windowId)->makeContextCurrent((windowPtr)windowId);
    }

    vertexBufferId windowManger::add(windowPieceId windowId, VertexBuffer *data)
    {
        bindContext(windowId);
        return findWinById(windowId)->getVertexBufferManger()->add(data);
    }

    indexBufferId windowManger::add(windowPieceId windowId, indexBuffer *data)
    {
        bindContext(windowId);
        return findWinById(windowId)->getIndexBufferManger()->add(data);
    }

    vertexArrayId windowManger::add(windowPieceId windowId, vertexArray *data)
    {
        bindContext(windowId);
        return findWinById(windowId)->getVertexArrayManger()->add(data);
    }

    shaderId windowManger::add(windowPieceId windowId, shader *data)
    {
        bindContext(windowId);
        return findWinById(windowId)->getShaderManger()->add(data);
    }

    

    void windowManger::bindVB(windowPieceId windowId, vertexBufferId id)
    {
        return findWinById(windowId)->getVertexBufferManger()->bind(id);
    }

    void windowManger::bindIB(windowPieceId windowId, indexBufferId id)
    {
        return findWinById(windowId)->getIndexBufferManger()->bind(id);
    }

    void windowManger::bindVA(windowPieceId windowId, vertexArrayId id)
    {
        return findWinById(windowId)->getVertexArrayManger()->bind(id);
    }

    void windowManger::bindS(windowPieceId windowId, shaderId id)
    {
        window *w = findWinById(windowId);
        shaderManger *sm;
        if(w != NULL)
            sm = w->getShaderManger();
        if(sm != NULL)
            sm->bind(id);
    }

    void windowManger::pushElement(windowPieceId windowId, vertexBufferId id, VertexBufferElement data)
    {
        findWinById(windowId)->getVertexBufferManger()->pushElement(id, data);
    }

    void windowManger::addBuffer(windowPieceId windowId, vertexArrayId id, vertexBufferId vbId)
    {
        bindContext(windowId);
        VertexBuffer *vb = static_cast<VertexBufferManger *>(findWinById(windowId)->getVertexBufferManger())->getVB(vbId);
        findWinById(windowId)->getVertexArrayManger()->addBuffer(id, vb);
    }

    shaderManger *windowManger::getShaderManger(windowPieceId windowId)
    {
        return static_cast<shaderManger *>(findWinById(windowId)->getShaderManger());
    }

    VertexBufferManger *windowManger::getVertexBufferManger(windowPieceId windowId)
    {
        return static_cast<VertexBufferManger *>(findWinById(windowId)->getVertexBufferManger());   
    }


    vertexArrayManger *windowManger::getVertexArrayManger(windowPieceId windowId)
    {
        return static_cast<vertexArrayManger *>(findWinById(windowId)->getVertexArrayManger());
        
    }

    uint32_t windowManger::getIndexBufferCount(windowPieceId windowId, indexBufferId id)
    {
        return findWinById(windowId)->getIndexBufferManger()->getCount(id);
    }

    renderApi *windowManger::getRenderApi(windowPieceId windowId)
    {
        return findWinById(windowId)->getRenderApi();
    }

    renderer *windowManger::getRenderer(windowPieceId windowId)
    {
        return findWinById(windowId)->getRenderer();
    }

    void windowManger::setCamera(windowPieceId windowId, coreCamera *cam)
    {
        findWinById(windowId)->setCamera(cam);
    }

}