#include "window.h"
#include "logger.h"
#include <string>


namespace raftelGraphicEngine
{
    bool windowManger::VSync = false;
    static std::vector<window> windows = std::vector<window>();

    void windowManger::setVSync(bool enabled){ window::setVSync(VSync = enabled); }

    void windowManger::init(){
        setVSync(true);
    }
    void windowManger::close(){}

    raftelId windowManger::addWindow(const std::string& title, bool useImGui, unsigned int width, unsigned int height)
    {
        window newWin(title, width, height, useImGui);
        newWin.Window =  newWin.Init(newWin);
        windows.push_back(newWin);
        return windows.size() - 1;
    }

    void windowManger::onUpdate(IEventData *sendor)
    {
        for(window win: windows)
        {
            win.onUpdate(win, sendor);
        }
    }

    windowPtr windowManger::raftelIdToWindowId(raftelId windowId)
    {
        return windows[windowId].Window;
    }

    unsigned int windowManger::getWidth(raftelId windowId){ return windows[windowId].Width; }
    unsigned int windowManger::getHeight(raftelId windowId){ return windows[windowId].Height; }
}