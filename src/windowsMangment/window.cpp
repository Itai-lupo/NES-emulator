#include "window.h"
#include "entity.h"
#include "logger.h"
#include <string>
#include <functional>
#include <algorithm>
// #include <boost/bind.hpp>

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

    windowPieceId windowManger::addWindow(const std::string& title, bool useImGui, unsigned int width, unsigned int height)
    {
        window *newWin = new window(title, width, height, useImGui);
        newWin->Window =  window::Init(newWin);
        windows.push_back(newWin);

        windowEntity *windowEntityData = new windowEntity(newWin);
        entityTaleId windowEntityId = entityManger::addEntity(dynamic_cast<windowEntity*>(windowEntityData));
        eventManger::addEvent(events::WindowResize, onWindowResize, windowEntityId, newWin->id);
        
        return newWin->id;
    }

    void windowManger::onUpdate(IEventData *sendor)
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

    unsigned int windowManger::getWidth(windowPieceId windowId)
    { 
        return (*std::find_if(
            windows.begin(), 
            windows.end(), 
            [=](window *win)-> bool { return win->id == windowId; }
        ))->Width;
    }
    unsigned int windowManger::getHeight(windowPieceId windowId)
    { 
        return (*std::find_if(
            windows.begin(), 
            windows.end(), 
            [=](window *win)-> bool { return win->id == windowId; }
        ))->Height;
    }
}