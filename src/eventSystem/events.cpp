#include "events.h"
#include <stdlib.h> 
#include "logger.h"

namespace LaughTaleEngine
{
    std::vector<event> *eventManger::eventList = new std::vector<event>[events::events_MAX];
    u_int32_t eventManger::nextEventId = 0;

    void eventManger::init()
    {
        eventManger::eventList = new std::vector<event>[events::events_MAX];
    }

    void eventManger:: close()
    {
        for (size_t i = 0; i < events::events_MAX; i++)
        {
            eventManger::eventList[i].clear();
        }
        
        eventManger::nextEventId = 0;
    }

    eventLaughId eventManger::addEvent(events eventType, eventCallbackFunc callback, entityTaleId entityID, windowPieceId windowID)
    {
        event *newEvent = new event(eventType, callback, entityID, windowID);
        return eventManger::addEvent(newEvent);
    }

    eventLaughId eventManger::addEvent(event *eventToAdd)
    {
        eventToAdd->id = eventManger::nextEventId;
        eventManger::nextEventId++;
        eventManger::eventList[eventToAdd->getEventType()].push_back(*eventToAdd); 
        return eventToAdd->id;
    }

    void eventManger::removeEvent(events eventType, eventLaughId eventToRemove)
    {
        for (uint64_t i = 0; i < eventManger::eventList[eventType].size(); i++)
        {
            if (eventManger::eventList[eventType][i].id == eventToRemove)
            {
                eventManger::eventList[eventType].erase(eventManger::eventList[eventType].begin() + i);
                return;
            }   
        }        
    }

    void eventManger::trigerEvent(events eventType, IEventData *sendor, windowPieceId windowID)
    {
        sendor->eventType = eventType;
        sendor->windowId = windowID;
        for(event e : eventManger::eventList[eventType]) 
        {
            if(windowID == 0 || e.getWindowID() == 0 || e.getWindowID() == windowID)
            {
                IEntity *a = entityManger::getEntityById(e.getEntityID()); 
                e.trigerEvent(a, sendor);
            }
        }
    }
}