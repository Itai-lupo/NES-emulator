#include "windowManger.h"
#include "logger.h"
#include <string>
#include <functional>
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64) // Windows

#elif defined(__ANDROID__)  // Android (implies Linux, so it must come first)

#elif defined(__linux__) // linux
    #include "linuxWindowFactory.h"
    #include "linuxInput.h"
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #if  TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1// Apple iOS
    
    #elif TARGET_OS_MAC == 1 // Apple OSX
    
    #endif
#else
    #error "platform not supported"
#endif

#define findWinById(windowId)                                           \
    std::find_if(                                                       \
            windows.begin(),                                            \
            windows.end(),                                              \
            [=](window *win)-> bool { return win->id == windowId; }     \
        )

namespace LTE
{
    std::vector<window*> windowManger::windows = std::vector<window*>();

    void windowManger::init(){
        windowFactory = new linuxWindowFactory();
    }
    
    void windowManger::close(){}    

    void windowManger::onWindowClose(gameObject *eventEntity, coreEventData *sendor)
    {        
        windows.erase(std::remove_if(
            windows.begin(), 
            windows.end(), 
            [=](window *win)-> bool { return win->id == sendor->windowId; }
        ), windows.end());

        delete sendor->win;
    }

    windowPieceId windowManger::addWindow(window *newWindow)
    {
        windows.push_back(newWindow);
                
        eventManger::startBuildingEvent()->
                setEventRoute("Window close/close window " + newWindow->Title + std::to_string(newWindow->id))->
                setEventCallback(onWindowClose)->
                setWindowId(newWindow->id)->add();

        
        return newWindow->id;
    }

    window *windowManger::getWindow(windowPieceId windowId)
    {
        auto temp = findWinById(windowId);
        if(temp != windows.end())
            return *temp;
        
        LAUGHTALE_ENGINR_LOG_ERROR("window wasn't found")
        return NULL;
    }

}