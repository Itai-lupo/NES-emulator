#pragma once


namespace LTE
{
    class window;
    
    
    struct coreEventData
    {
        coreEventData(): route("no route"){}

        coreEventData(std::string route): route(route){}
        virtual ~coreEventData() {}
        std::string route;
        windowPieceId windowId = 0;
        window *win = nullptr;
        eventLaughId id;
    };
}