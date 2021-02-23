#pragma once
#include <functional>
#include <vector>
#include "entity.h"


namespace raftelGraphicEngine
{
    struct event;
    struct IEventData;

    typedef std::function<void(IEntity *eventEntity, IEventData *sendor)> eventCallbackFunc;


    enum events
    {
        manualEvent,
        events_MAX
    };


    struct IEventData
    {
        public:
            virtual ~IEventData() {}
            events eventType;
    };
    

    struct event
    {
        public:
            event(events eventType,  eventCallbackFunc callback, int entityID)
                :eventType(eventType), callback(callback), entityID(entityID){};

            void trigerEvent(IEntity *eventEntity, IEventData *sendor)
            {
                this->callback(eventEntity, sendor);
            }

            events getEventType() { return eventType; }
            int getEntityID() { return entityID; }

        private:
            events eventType;
            eventCallbackFunc  callback;
            int entityID;
    };

    class eventManger
    {
        private:
            static std::vector<event> *eventList;
        public:
            static void init();
            static void close();
            static int addEvent(events eventType, eventCallbackFunc callback, int entityID);
            static int addEvent(event *eventToAdd);
            static void trigerEvent(events eventType, IEventData *sendor);
    };


}