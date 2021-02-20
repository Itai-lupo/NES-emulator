#include "events.h"
#include <stdlib.h> 
#include "logger.h"

namespace raftelGraphicEngine
{
    std::vector<event> *eventManger::eventList = new std::vector<event>[events::events_MAX];


    void eventManger::init()
    {

    }

    void eventManger:: close()
    {
        
    }

    int eventManger::addEvent(events eventType, std::function<void(IEntity eventEntity, IEventData sendor)> callback, int entityID)
    {
        event *newEvent = new event(eventType, callback, entityID);
        return eventManger::addEvent(newEvent);
    }

    int eventManger::addEvent(event *eventToAdd)
    {
        eventManger::eventList[eventToAdd->getEventType()].push_back(*eventToAdd); 
        return eventManger::eventList[eventToAdd->getEventType()].size() - 1;
    }

    void eventManger::trigerEvent(events eventType, IEventData sendor)
    {
        for (event e : eventManger::eventList[eventType]) {
            IEntity a(e.getEntityID()); //todo: replace with get entity by id 
            e.trigerEvent(a, sendor);
        }
    }
}