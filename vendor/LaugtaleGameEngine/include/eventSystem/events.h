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
#include "router.h"

namespace LTE
{
    typedef std::function<void(gameObject *eventEntity, coreEventData *sendor)> eventCallbackFunc;
    class eventManger;

    struct event
    {
        public:
            friend class eventManger;
            
            void trigerEvent(gameObject *eventEntity, coreEventData *sendor)
            {
                this->callback(eventEntity, sendor);
            }

            std::string getEventRoute() { return route; }
            entityTaleId getEntityID() { return entityID; }
            windowPieceId getWindowID() { return window; }
            eventLaughId getID() { return id; }

        private:
            eventLaughId id;
            windowPieceId window = 0;
            std::string route;
            eventCallbackFunc callback;
            entityTaleId entityID = -1;

            event(){}
    };

    class eventManger
    {
        
        private:
            class eventBuilder
            {
                private:
                    event *product;
                    eventBuilder(){}

                    void reset(){ product = new event(); }
                    
                    event *getProduct(){ return product; }
                    friend class eventManger;

                public:
                    eventBuilder *setWindowId(windowPieceId window)
                    {
                        product->window = window;
                        return this;
                    }
                    
                    eventBuilder *setEventRoute(const std::string& route)
                    {
                        product->route = route;
                        return this;
                    }
                    
                    eventBuilder *setEventCallback(const eventCallbackFunc&  callback)
                    {
                        product->callback = callback;
                        return this;
                    }
                    
                    eventBuilder *setEntityID(entityTaleId entityID)
                    {
                        product->entityID = entityID;
                        return this;
                    }

                    eventLaughId add()
                    {
                        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("you need to set event route", product->route == "");
                        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("you need to set event callback", !product->callback);

                        return addEvent();
                    }
                };

            static LTE::router<event*> eventList;
            static uint32_t nextEventId;
            static inline eventBuilder builder;
            static eventLaughId addEvent();
        public:
            static void init();
            static void close();
            static eventBuilder *startBuildingEvent();
            static void addCoustemEventsRoute(const std::string& routeToAdd);
            static void removeEvent(const std::string& eventRoute);
            static void trigerEvent(coreEventData *sendor);
    };
}