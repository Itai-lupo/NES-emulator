#include "events.h"
#include <stdlib.h> 
#include "logger.h"

namespace raftelGraphicEngine
{
    std::vector<event> *eventManger::eventList = new std::vector<event>[events::events_MAX];


    void eventManger::init()
    {
        eventManger::eventList = new std::vector<event>[events::events_MAX];
    }

    void eventManger:: close()
    {
        
    }

    raftelId eventManger::addEvent(events eventType, eventCallbackFunc callback, raftelId entityID, windowPtr windowID)
    {
        event *newEvent = new event(eventType, callback, entityID, windowID);
        return eventManger::addEvent(newEvent);
    }

    raftelId eventManger::addEvent(event *eventToAdd)
    {
        eventManger::eventList[eventToAdd->getEventType()].push_back(*eventToAdd); 
        eventToAdd->id = eventManger::eventList[eventToAdd->getEventType()].size() - 1;
        return eventToAdd->id;
    }

    void eventManger::trigerEvent(events eventType, IEventData *sendor, windowPtr windowID)
    {
        sendor->eventType = eventType;
        for(event e : eventManger::eventList[eventType]) 
        {
            if(windowID == NULL || e.getWindowID() == NULL || e.getWindowID() == windowID)
            {
                IEntity *a = entityManger::getEntityById(e.getEntityID()); 
                e.trigerEvent(a, sendor);
            }
        }
    }
}