#include "events.h"
#include <stdlib.h> 
#include "logger.h"
#include "windowManger.h"
#include "window.h"
#include "LTEError.h"

namespace LTE
{
    LTE::router<event*> eventManger::eventList;
    u_int32_t eventManger::nextEventId = 0;

    void eventManger::init()
    {
        eventList.addRoute("Window close/");
        eventList.addRoute("Window resize/");
        eventList.addRoute("Window focus/");
        eventList.addRoute("Window lost focus/");
        eventList.addRoute("Window moved/");

        eventList.addRoute("App tick/");
        eventList.addRoute("App update/");
        eventList.addRoute("App render/");
        
        eventList.addRoute("Key pressed/");
        eventList.addRoute("Key released/");
        eventList.addRoute("Key repeat/");
        eventList.addRoute("Key typed/");
        
        eventList.addRoute("Mouse button pressed/");
        eventList.addRoute("Mouse button released/");
        eventList.addRoute("Mouse moved/");
        eventList.addRoute("Mouse scrolled/");
        
        eventList.addRoute("ImGui render/");
        
        eventList.addRoute("server connection/");
        eventList.addRoute("message received/");
        eventList.addRoute("message sent/");
        
        
    }

    void eventManger:: close()
    {
        
        eventManger::eventList.clearAndDelete();
        
        eventManger::nextEventId = 0;
    }

    eventManger::eventBuilder *eventManger::startBuildingEvent()
    {
        builder.reset();
        return &builder;
    }

    void eventManger::addCoustemEventsRoute(const std::string& routeToAdd)
    {
        eventManger::eventList.addRoute(routeToAdd);
    }

    eventLaughId eventManger::addEvent()
    {
        event *eventToAdd = builder.getProduct();
        eventToAdd->id = eventManger::nextEventId;
        eventManger::nextEventId++;
        eventManger::eventList.addValue(eventToAdd->getEventRoute(), eventToAdd); 
        return eventToAdd->id;
    }

    void eventManger::removeEvent(const std::string& eventRoute)
    {
        eventList.deleteValue(eventRoute);
    }

    void eventManger::trigerEvent(coreEventData *sendor)
    {
        if(sendor->route == "")
            LAUGHTALE_ENGINR_LOG_FATAL("please specify event route");

        std::string route = sendor->route;
        windowPieceId winId = sendor->windowId;

        eventManger::eventList.itrateFrom([&](event *e){
            try{
                if(winId == 0 || e->getWindowID() == 0 || e->getWindowID() == winId)
                {
                    if(e->getWindowID() != 0 && winId == 0){
                        sendor->windowId = e->getWindowID(); 
                    }

                    if(sendor->windowId)
                        sendor->win = windowManger::getWindow(sendor->windowId);
                    
                    sendor->route = e->getEventRoute();
                    sendor->id = e->id;
                    
                    gameObject *eventObject = nullptr;

                    if(e->getEntityID() != (entityTaleId)-1 )
                        eventObject = entityManger::getEntityById(e->getEntityID()); 
                    
                    e->trigerEvent(eventObject, sendor);

                }
            }
            catch(GameObjectNotFoundException* ex)
            {
                LAUGHTALE_ENGINR_LOG_WARNING("entity with id: " << e->getEntityID() << " was'nt found so faild to call event with route: " << e->route);
                removeEvent(e->route);
            }
            catch(WindowNotFoundException* ex)
            {
                LAUGHTALE_ENGINR_LOG_ERROR("faild to call event " << e->getEventRoute() << " because window with id " << e->getWindowID() << " wasn't found");
                removeEvent(e->route);
            }
            catch(const std::exception& ex)
            {
                LAUGHTALE_ENGINR_LOG_ERROR("faild to call event " << e->getEventRoute() << " because: " << ex.what());
            }
        }, sendor->route);

        sendor->windowId = winId;
        sendor->route = route;
    }
}