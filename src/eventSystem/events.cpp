#include "events.h"
#include <stdlib.h> 
#include "logger.h"

namespace LTE
{
    std::vector<event*> *eventManger::eventList;
    u_int32_t eventManger::nextEventId = 0;

    void eventManger::init()
    {
        eventManger::eventList = new std::vector<event*>[events::events_MAX];
    }

    void eventManger:: close()
    {
        for (size_t i = 0; i < events::events_MAX; i++)
        {
            eventManger::eventList[i].clear();
        }
        
        eventManger::nextEventId = 0;
    }

    eventLaughId eventManger::addEvent(event *eventToAdd)
    {
        eventToAdd->id = eventManger::nextEventId;
        eventManger::nextEventId++;
        eventManger::eventList[eventToAdd->getEventType()].push_back(eventToAdd); 
        return eventToAdd->id;
    }

    void eventManger::removeEvent(events eventType, eventLaughId eventToRemove)
    {
        for (uint64_t i = 0; i < eventManger::eventList[eventType].size(); i++)
        {
            if (eventManger::eventList[eventType][i]->id == eventToRemove)
            {
                eventManger::eventList[eventType].erase(eventManger::eventList[eventType].begin() + i);
                return;
            }   
        }        
        LAUGHTALE_ENGINR_LOG_WARNING("faild to remove event");
    }

    void eventManger::trigerEvent(coreEventData *sendor)
    {
        if(!sendor->eventType)
            LAUGHTALE_ENGINR_LOG_FATAL("please Spcifay event type" << sendor->eventType);

        events type = sendor->eventType;
        for(event *e : eventManger::eventList[type]) 
        {
            if(sendor->windowId == 0 || e->getWindowID() == 0 || e->getWindowID() == sendor->windowId)
            {
                sendor->id = e->id;
                IEntity *a = entityManger::getEntityById(e->getEntityID()); 
                if((e->getEntityID() != -1 && a != nullptr) || e->getEntityID() == (entityTaleId)-1)
                    e->trigerEvent(a, sendor);
                else
                    LAUGHTALE_ENGINR_LOG_ERROR("entity wasnt found:" << e->getEntityID() << ", " << e->id);

            }
        }
    }
}