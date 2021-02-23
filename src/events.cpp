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

    int eventManger::addEvent(events eventType, eventCallbackFunc callback, int entityID)
    {
        event *newEvent = new event(eventType, callback, entityID);
        return eventManger::addEvent(newEvent);
    }

    int eventManger::addEvent(event *eventToAdd)
    {
        eventManger::eventList[eventToAdd->getEventType()].push_back(*eventToAdd); 
        return eventManger::eventList[eventToAdd->getEventType()].size() - 1;
    }

    void eventManger::trigerEvent(events eventType, IEventData *sendor)
    {
        sendor->eventType = eventType;
        for(event e : eventManger::eventList[eventType]) 
        {
            IEntity *a = entityManger::getEntityById(e.getEntityID()); 
            e.trigerEvent(a, sendor);
        }
    }
}