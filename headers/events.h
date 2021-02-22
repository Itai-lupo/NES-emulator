#pragma once
#include <functional>
#include <vector>
#include "entity.h"

namespace raftelGraphicEngine
{
    enum events
    {
        manualEvent,
        events_MAX
    };


    class IEventData
    {
        public:
            IEventData(int id):id(id){}
            int getId() { return id; }
        private:
            int id;
    };
    

    struct event
    {
        public:
            event(events eventType, std::function<void(IEntity *eventEntity, IEventData sendor)> callback, int entityID)
                :eventType(eventType), callback(callback), entityID(entityID){};

            void trigerEvent(IEntity *eventEntity, IEventData sendor)
            {
                this->callback(eventEntity, sendor);
            }

            events getEventType() { return eventType; }
            int getEntityID() { return entityID; }

        private:
            events eventType;
            std::function<void(IEntity *eventEntity, IEventData sendor)> callback;
            int entityID;
    };

    class eventManger
    {
        private:
            static std::vector<event> *eventList;
        public:
            static void init();
            static void close();
            static int addEvent(events eventType, std::function<void(IEntity *eventEntity, IEventData sendor)> callback, int entityID);
            static int addEvent(event *eventToAdd);
            static void trigerEvent(events eventType, IEventData sendor);
    };

}