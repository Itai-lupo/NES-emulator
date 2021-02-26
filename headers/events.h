#pragma once
#include <functional>
#include <vector>
#include "core.h"
#include "entity.h"

#include <GLFW/glfw3.h>


namespace raftelGraphicEngine
{
    struct event;
    struct IEventData;
    typedef std::function<void(IEntity *eventEntity, IEventData *sendor)> eventCallbackFunc;

    enum events
    {
        manualEvent,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyRepeat,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
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
            windowWidth(width), windowHeight(height), IEventData(events::WindowResize), window(window){}

        int windowWidth, windowHeight;
        windowPtr window;
    };
    
    
    struct KeyData: public IEventData
    {
        KeyData(int key, int scancode, int mods, windowPtr window):
            key(key), scancode(scancode), mods(mods), IEventData(), window(window){}

        int key, scancode, mods;
        windowPtr window;

    };
    
    struct mouseClickData: public IEventData
    {
        mouseClickData(int button, int mods, windowPtr window):
            button(button), mods(mods), IEventData(), window(window){}

        int button, mods;
        windowPtr window;

    };

    struct mouseScrollData: public IEventData
    {
        mouseScrollData(double xOffset, double yOffset, windowPtr window):
            xOffset(xOffset), yOffset(yOffset), IEventData(events::MouseScrolled), window(window){}

        double xOffset, yOffset;
        windowPtr window;

    };

    struct mouseMoveData: public IEventData
    {
        mouseMoveData(double xPos, double yPos, windowPtr window):
            xPos(xPos), yPos(yPos), window(window), IEventData(events::MouseScrolled){}

        double xPos, yPos;
        windowPtr window;
    };
    
    struct event
    {
        public:
            event(events eventType,  eventCallbackFunc callback, int entityID, windowPtr window)
                :eventType(eventType), callback(callback), entityID(entityID), window(window){};

            void trigerEvent(IEntity *eventEntity, IEventData *sendor)
            {
                this->callback(eventEntity, sendor);
            }

            events getEventType() { return eventType; }
            raftelId getEntityID() { return entityID; }
            windowPtr getWindowID() { return window; }

            raftelId id;
        private:
            windowPtr window;
            events eventType;
            eventCallbackFunc  callback;
            raftelId entityID;
    };

    class eventManger
    {
        private:
            static std::vector<event> *eventList;
        public:
            static void init();
            static void close();
            static raftelId addEvent(events eventType, eventCallbackFunc callback, int entityID, windowPtr windowID = NULL);
            static raftelId addEvent(event *eventToAdd);
            static void trigerEvent(events eventType, IEventData *sendor, windowPtr windowID = NULL);
    };
}