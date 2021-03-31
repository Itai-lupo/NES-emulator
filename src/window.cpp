#include "window.h"
#include "entity.h"
#include "logger.h"
#include <string>
#include <functional>

class windowEntity: public raftelGraphicEngine::IEntity
{
    public:
        windowEntity(window *win): win(win) {}
        window *win;
};

namespace raftelGraphicEngine
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
        logger::LogInfo("event width: " + std::to_string(sendorData->windowWidth) + " hight: " + std::to_string(sendorData->windowHeight));
        logger::LogInfo("entity width: " + std::to_string(entityData->win->Width) + " hight: " + std::to_string(entityData->win->Height));

        entityData->win->Width = sendorData->windowWidth;
        entityData->win->Height = sendorData->windowHeight;
    }

    raftelId windowManger::addWindow(const std::string& title, bool useImGui, unsigned int width, unsigned int height)
    {
        window *newWin = new window(title, width, height, useImGui);
        newWin->Window =  window::Init(newWin);
        windows.push_back(newWin);

        windowEntity *windowEntityData = new windowEntity(newWin);
        raftelId windowEntityId = entityManger::addEntity(dynamic_cast<windowEntity*>(windowEntityData));
        eventManger::addEvent(events::WindowResize, onWindowResize, windowEntityId, newWin->Window);
        
        return windows.size() - 1;
    }

    void windowManger::onUpdate(IEventData *sendor)
    {
        logger::LogInfo("on update");
        for(window *win: windows)
        {
            logger::LogInfo(win->Title);
            win->onUpdate(win, sendor);
        }
    }

    windowPtr windowManger::raftelIdToWindowId(raftelId windowId)
    {
        return windows[windowId]->Window;
    }

    unsigned int windowManger::getWidth(raftelId windowId){ return windows[windowId]->Width; }
    unsigned int windowManger::getHeight(raftelId windowId){ return windows[windowId]->Height; }
}