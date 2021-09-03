#include "windowManger.h"
#include "logger.h"
#include <string>
#include <functional>
#include <algorithm>



namespace LTE
{
    std::vector<window*> windowManger::windows = std::vector<window*>();

    void windowManger::init()
    {
        buildWindow = new windowBuilder();
    }
    
    void windowManger::close()
    {
        delete buildWindow;
    }    

    void windowManger::onWindowClose(gameObject *eventEntity, coreEventData *sendor)
    {        
        windows.erase(std::remove_if(
            windows.begin(), 
            windows.end(), 
            [=](window *win)-> bool { return win->id == sendor->windowId; }
        ), windows.end());

        delete sendor->win;
    }


    windowPieceId windowManger::addWindow(std::function<void(windowBuilder *Builder)> build)
    {
        buildWindow->reset();
        build(buildWindow);
        
        window *newWindow = buildWindow->build();

        windows.push_back(newWindow);
                
        eventManger::startBuildingEvent()->
                setEventRoute("Window close/close window " + newWindow->Title + std::to_string(newWindow->id))->
                setEventCallback(onWindowClose)->
                setWindowId(newWindow->id)->add();

        
        return newWindow->id;
    }

    window *windowManger::getWindow(windowPieceId windowId)
    {
        auto temp = std::find_if(                                                       
                windows.begin(),                                            
                windows.end(),                                              
                [=](window *win)-> bool { return win->id == windowId; }     
            );

        if(temp != windows.end())
            return *temp;
        
        LAUGHTALE_ENGINR_LOG_ERROR("window wasn't found")
        return NULL;
    }

}