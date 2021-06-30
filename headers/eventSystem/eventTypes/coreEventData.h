#pragma once
#include "coreEventData.h"

namespace LTE
{
    enum events
    {
        noEvent,
        manualEvent,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyRepeat, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        ImGuiRender,
        serverConnection, messageReceived, messageSent, 
        events_MAX
    };
    
    struct coreEventData
    {
        coreEventData(): eventType(noEvent){}

        coreEventData(events eventType): eventType(eventType){}
        virtual ~coreEventData() {}
        events eventType;
        windowPieceId windowId = 0;
        eventLaughId id;
    };
}