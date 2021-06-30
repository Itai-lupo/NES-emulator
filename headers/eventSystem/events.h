#pragma once
#include <functional>
#include <vector>
#include "core.h"
#include "entity.h"
#include "coreEventData.h"
#include "mouseMoveEventData.h"
#include "KeyData.h"
#include "WindowResizeData.h"
#include "keyTypedData.h"
#include "mouseClickData.h"
#include "mouseScrollData.h"
#include "onUpdateData.h"
#include "logger.h"

namespace LTE
{
    typedef std::function<void(IEntity *eventEntity, coreEventData *sendor)> eventCallbackFunc;
    
    struct event
    {
        public:
            class eventBuilder
            {
                private:
                    static inline eventBuilder *singleton = nullptr;
                    event *product;
                    eventBuilder(){}
                public:
                    static eventBuilder *startBuilding()
                    {
                        if(singleton == nullptr)
                            singleton = new eventBuilder();
                        
                        singleton->rest();
                        return singleton;
                    }

                    eventBuilder *rest()
                    {
                        product = new event();
                        return singleton;
                    }

                    eventBuilder *setWindowId(windowPieceId window)
                    {
                        product->window = window;
                        return singleton;
                    }
                    
                    eventBuilder *setEventType(events eventType)
                    {
                        product->eventType = eventType;
                        return singleton;
                    }
                    
                    eventBuilder *setEventCallback(eventCallbackFunc  callback)
                    {
                        product->callback = callback;
                        return singleton;
                    }
                    
                    eventBuilder *setEntityID(entityTaleId entityID)
                    {
                        product->entityID = entityID;
                        return singleton;
                    }

                    event *build()
                    {
                        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("you need to set event type", product->eventType == events::noEvent);
                        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("you need to set event callback", !product->callback);

                        return product;
                    }
            };

            
            void trigerEvent(IEntity *eventEntity, coreEventData *sendor)
            {
                this->callback(eventEntity, sendor);
            }

            events getEventType() { return eventType; }
            entityTaleId getEntityID() { return entityID; }
            windowPieceId getWindowID() { return window; }

            eventLaughId id;
        private:
            windowPieceId window = 0;
            events eventType;
            eventCallbackFunc callback;
            entityTaleId entityID = -1;

            event(){}
    };

    class eventManger
    {
        private:
            static std::vector<event*> *eventList;
            static uint32_t nextEventId;
        public:
            static void init();
            static void close();
            static eventLaughId addEvent(event *eventToAdd);
            static void removeEvent(events eventType, eventLaughId eventToRemove);
            static void trigerEvent(coreEventData *sendor);
    };
}