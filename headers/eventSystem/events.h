#pragma once
#include <functional>
#include <vector>
#include "core.h"
#include "entity.h"



namespace LaughTaleEngine
{
    struct event;
    struct IEventData;
    typedef std::function<void(IEntity *eventEntity, IEventData *sendor)> eventCallbackFunc;

    enum events
    {
        manualEvent,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyRepeat, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        ImGuiRender,
        events_MAX
    };

    struct IEventData
    {
        IEventData(): eventType(manualEvent){}

        IEventData(events eventType): eventType(eventType){}
        virtual ~IEventData() {}
        events eventType;
        windowPtr window = NULL;
    };

    struct WindowResizeData: public IEventData
    {
        WindowResizeData(int width, int height, windowPtr window):
            IEventData(events::WindowResize), windowWidth(width), windowHeight(height), window(window){}

        int windowWidth, windowHeight;
        windowPtr window;
    };
    
    
    struct KeyData: public IEventData
    {
        KeyData(int key, int scancode, int mods, windowPtr window):
            IEventData(), key(key), scancode(scancode), mods(mods), window(window){}

        int key, scancode, mods;
        windowPtr window;

    };

    struct keyTypedData: public IEventData
    {
        keyTypedData(unsigned int keycode, windowPtr window):
            keycode(keycode), window(window){}

        unsigned int keycode;
        windowPtr window;

    };
    
    struct mouseClickData: public IEventData
    {
        mouseClickData(int button, int mods, windowPtr window):
            IEventData(), button(button), mods(mods), window(window){}

        int button, mods;
        windowPtr window;

    };

    struct mouseScrollData: public IEventData
    {
        mouseScrollData(double xOffset, double yOffset, windowPtr window):
            IEventData(events::MouseScrolled), xOffset(xOffset), yOffset(yOffset), window(window){}

        double xOffset, yOffset;
        windowPtr window;

    };

    struct mouseMoveData: public IEventData
    {
        mouseMoveData(double xPos, double yPos, windowPtr window):
            IEventData(events::MouseScrolled), xPos(xPos), yPos(yPos), window(window) {}

        double xPos, yPos;
        windowPtr window;
    };

    struct onUpdateData: public IEventData
    {
        onUpdateData(uint64_t startTime, uint64_t currentTime, short DeltaTime):
            IEventData(events::AppUpdate), startTime(startTime), currentTime(currentTime), DeltaTime(DeltaTime){}

        uint64_t startTime, currentTime;
        short DeltaTime;
    };
    
    struct event
    {
        public:
            event(events eventType,  eventCallbackFunc callback, int entityID, windowPtr window)
                :window(window), eventType(eventType), callback(callback), entityID(entityID){};

            void trigerEvent(IEntity *eventEntity, IEventData *sendor)
            {
                this->callback(eventEntity, sendor);
            }

            events getEventType() { return eventType; }
            entityTaleId getEntityID() { return entityID; }
            windowPtr getWindowID() { return window; }

            eventLaughId id;
        private:
            windowPtr window;
            events eventType;
            eventCallbackFunc  callback;
            entityTaleId entityID;
    };

    class eventManger
    {
        private:
            static std::vector<event> *eventList;
            static u_int32_t nextEventId;
        public:
            static void init();
            static void close();
            static eventLaughId addEvent(events eventType, eventCallbackFunc callback, entityTaleId entityID, windowPtr windowID = NULL);
            static eventLaughId addEvent(event *eventToAdd);
            static void trigerEvent(events eventType, IEventData *sendor, windowPtr windowID = NULL);
            static void removeEvent(events eventType, eventLaughId eventToRemove);

    };
}