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

    void windowManger::setVSync(bool enabled){ window::setVSync(enabled); VSync = enabled; }

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
        entityData->win->getRenderApi()->SetViewport(0, 0, sendorData->windowWidth, sendorData->windowHeight);
    }

    void onWindowClose(IEntity *eventEntity, IEventData *sendor)
    {
        findWinById(sendor->windowId)->Shutdown(windowManger::getWindow(sendor->windowId));

        windows.erase(std::remove_if(
            windows.begin(), 
            windows.end(), 
            [=](window *win)-> bool { return win->id == sendor->windowId; }
        ), windows.end());
    }

    windowPieceId windowManger::addWindow(const std::string& title, bool useImGui, unsigned int width, unsigned int height, renderAPI renderAPIType)
    {
        window *newWin = new window(title, width, height, useImGui, renderAPIType);
        window::Init(newWin);
        windows.push_back(newWin);

        windowEntity *windowEntityData = new windowEntity(newWin);
        entityTaleId windowEntityId = entityManger::addEntity(dynamic_cast<windowEntity*>(windowEntityData));
        eventManger::addEvent(events::WindowResize, onWindowResize, windowEntityId, newWin->id);
        eventManger::addEvent(events::WindowClose, onWindowClose, -1, newWin->id);
        
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
        window *w = findWinById(windowId);
        if(w != nullptr)            
            return findWinById(windowId)->Width;
        LAUGHTALE_ENGINR_LOG_ERROR("no window with id: " << windowId);
        return -1;
    }
    unsigned int windowManger::getHeight(windowPieceId windowId)
    { 

        window *w = findWinById(windowId);
        if(w != nullptr)            
            return findWinById(windowId)->Height;
        LAUGHTALE_ENGINR_LOG_ERROR("no window with id: " << windowId);
        return -1;
    }

    void windowManger::bindContext(windowPieceId windowId)
    {
        window *w = findWinById(windowId);
        if(w != nullptr)
            w->makeContextCurrent((windowPtr)windowId);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind Context: no window with id: " << windowId, w == nullptr);
        
    }

    vertexBufferId windowManger::add(windowPieceId windowId, VertexBuffer *data)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        VertexBufferManger *vbm = nullptr;
        if(w != nullptr)
            vbm = w->getVertexBufferManger();
        if(vbm != nullptr)
            return vbm->add(data);

        LAUGHTALE_ENGINR_LOG_ERROR("faild to add vb: no window with id: " << windowId);
        return -1;
    }

    indexBufferId windowManger::add(windowPieceId windowId, indexBuffer *data)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        indexBufferManger *m = nullptr;
        if(w != nullptr)
            m = w->getIndexBufferManger();
        if(m != nullptr)
            return m->add(data);

        LAUGHTALE_ENGINR_LOG_ERROR("faild to add index buffer: no window with id: " << windowId);
        return -1;
    }

    vertexArrayId windowManger::add(windowPieceId windowId, vertexArray *data)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        vertexArrayManger *m = nullptr;
        if(w != nullptr)
            m = w->getVertexArrayManger();
        if(m != nullptr)
            return m->add(data);

        LAUGHTALE_ENGINR_LOG_ERROR("faild to add vertex array: no window with id: " << windowId);
        return -1;
    }

    shaderId windowManger::add(windowPieceId windowId, shader *data)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        shaderManger *m = nullptr;
        if(w != nullptr)
            m = w->getShaderManger();
        if(m != nullptr)
            return m->add(data);

        LAUGHTALE_ENGINR_LOG_ERROR("faild to add shader: no window with id: " << windowId);
        return -1;
    }

    

    void windowManger::bindVB(windowPieceId windowId, vertexBufferId id)
    {
        window *w = findWinById(windowId);
        VertexBufferManger *vbm = nullptr;
        if(w != nullptr)
            vbm = w->getVertexBufferManger();
        if(vbm != nullptr)
            vbm->bind(id);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind vb: no window with id: " << windowId, w == nullptr);
    }

    void windowManger::bindIB(windowPieceId windowId, indexBufferId id)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        indexBufferManger *m = nullptr;
        if(w != nullptr)
            m = w->getIndexBufferManger();
        if(m != nullptr)
            m->bind(id);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind index buffer: no window with id: " << windowId, w == nullptr);
    }

    void windowManger::bindVA(windowPieceId windowId, vertexArrayId id)
    {

        bindContext(windowId);
        window *w = findWinById(windowId);
        vertexArrayManger *m = nullptr;
        if(w != nullptr)
            m = w->getVertexArrayManger();
        if(m != nullptr)
            return m->bind(id);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind vertex array: no window with id: " << windowId, w == nullptr);
        return findWinById(windowId)->getVertexArrayManger()->bind(id);
    }

    void windowManger::bindS(windowPieceId windowId, shaderId id)
    {
        window *w = findWinById(windowId);
        shaderManger *sm = nullptr;
        if(w != nullptr)
            sm = w->getShaderManger();
        if(sm != nullptr)
            sm->bind(id);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind vertex array: no window with id: " << windowId, w == nullptr);
        
    }

    void windowManger::pushElement(windowPieceId windowId, vertexBufferId id, VertexBufferElement data)
    {
        window *w = findWinById(windowId);
        VertexBufferManger *vbm = nullptr;
        if(w != nullptr)
            vbm = w->getVertexBufferManger();
        if(vbm != nullptr)
            vbm->pushElement(id, data);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to pushElement vb: no window with id: " << windowId, w == nullptr);
    }

    void windowManger::addBuffer(windowPieceId windowId, vertexArrayId id, vertexBufferId vbId)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        VertexBufferManger *vbm = nullptr;
        vertexArrayManger *vam = nullptr;
        VertexBuffer *vb = nullptr;
        
        if(w != nullptr)
        {
            vbm = w->getVertexBufferManger();
            vam = w->getVertexArrayManger();
        }
        if(vbm != nullptr)
            vb = vbm->getVB(vbId);
        if(vam != nullptr && vb != nullptr)
            vam->addBuffer(id, vb);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to addBuffer vb: no window with id: " << windowId, w == nullptr);
    }

    shaderManger *windowManger::getShaderManger(windowPieceId windowId)
    {
        window *w = findWinById(windowId);
        if(w != nullptr) return w->getShaderManger();
        return nullptr;
    }

    VertexBufferManger *windowManger::getVertexBufferManger(windowPieceId windowId)
    {
        window *w = findWinById(windowId);
        if(w != nullptr) return w->getVertexBufferManger();
        return nullptr;   
    }


    vertexArrayManger *windowManger::getVertexArrayManger(windowPieceId windowId)
    {
        window *w = findWinById(windowId);
        if(w != nullptr) return w->getVertexArrayManger();
        return nullptr;
        
    }

    uint32_t windowManger::getIndexBufferCount(windowPieceId windowId, indexBufferId id)
    {
        bindContext(windowId);
        window *w = findWinById(windowId);
        indexBufferManger *m = nullptr;
        if(w != nullptr)
            m = w->getIndexBufferManger();
        if(m != nullptr)
            return m->getCount(id);

        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to found index buffer: no window with id: " << windowId, w == nullptr);
        return 0;
    }

    renderApi *windowManger::getRenderApi(windowPieceId windowId)
    {
        window *w = findWinById(windowId);
        if(w != nullptr) return w->getRenderApi();
        return nullptr;
    }

    renderer *windowManger::getRenderer(windowPieceId windowId)
    {
        window *w = findWinById(windowId);
        if(w != nullptr) return w->getRenderer();
        return nullptr;
    }

    void windowManger::setCamera(windowPieceId windowId, coreCameraControler *cam)
    {
        
        window *w = findWinById(windowId);
        if(w == nullptr) return; 
        w->setCamera(cam);
    }

}