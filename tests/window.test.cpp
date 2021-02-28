
#include "raftelGraphicEngine.h"
#include <gtest/gtest.h>
#include <string.h>
#include "glad.h"

using namespace raftelGraphicEngine;

// TEST(window, openWindow)
// {
//     windowManger::addWindow();
//     windowManger::addWindow();

//     while (true)
//     {
//         glClearColor(1, 0, 1, 1);
//         glClear(GL_COLOR_BUFFER_BIT);
//         windowManger::onUpdate();
//     }
    
// }


// TEST(window, openWindowAndUseEvent)
// {
//     eventCallbackFunc mouseMoveDatacallback = [](IEntity *eventEntity, IEventData *sendor){
//         mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
//         std::string msg = "mouseMoveDatacallback" + std::to_string(sendorData->xPos) + ", " + std::to_string(sendorData->yPos);
//         logger::LogInfo(msg);
//     };


//     eventCallbackFunc mouseScrollDatacallback = [](IEntity *eventEntity, IEventData *sendor){
//         mouseScrollData* sendorData = dynamic_cast<mouseScrollData*>(sendor);
//         std::string msg = "mouseMoveDatacallback" + std::to_string(sendorData->xOffset) + ", " + std::to_string(sendorData->yOffset);
//         logger::LogInfo(msg);
//     };
    
    
//     init();

//     windowManger::addWindow();

//     IEntity testEntity;
//     raftelId id = entityManger::addEntity(&testEntity);
//     eventManger::addEvent(events::MouseMoved, mouseMoveDatacallback, id);
//     eventManger::addEvent(events::MouseScrolled, mouseScrollDatacallback, id);

//     std::string msg("event added sucssfly");
//     logger::LogInfo(msg);

    
//     while (true)
//     {
//         glClearColor(1, 0, 1, 1);
//         glClear(GL_COLOR_BUFFER_BIT);
//         windowManger::onUpdate();
//     }
    
// }


TEST(window, openWindowAndUseEvent)
{
    eventCallbackFunc mouseMoveDatacallbackWin1 = [](IEntity *eventEntity, IEventData *sendor){
        mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
        std::string msg = "mouseMoveDatacallbackWin1: " + std::to_string(sendorData->xPos) + ", " + std::to_string(sendorData->yPos);
        logger::LogInfo(msg);
    };


    eventCallbackFunc mouseMoveDatacallbackWin2 = [](IEntity *eventEntity, IEventData *sendor){
        mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
        std::string msg = "mouseMoveDatacallbackWin2: " + std::to_string(sendorData->xPos) + ", " + std::to_string(sendorData->yPos);
        logger::LogInfo(msg);
    };


    eventCallbackFunc WindowClose = [](IEntity *eventEntity, IEventData *sendor){
        app::keepRunning = false;
    };

    eventCallbackFunc onRenderWin1 = [](IEntity *eventEntity, IEventData *sendor){
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    eventCallbackFunc onRenderWin2 = [](IEntity *eventEntity, IEventData *sendor){
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    };
    
    
    app::init();

    raftelId win1 =  windowManger::addWindow("win 1");
    raftelId win2 = windowManger::addWindow("win 2");

    std::string msg = "win1: " + std::to_string(win1) + ", win2: " + std::to_string(win2);
    logger::LogInfo(msg);
    
    IEntity testEntity;
    raftelId id = entityManger::addEntity(&testEntity);
    eventManger::addEvent(events::MouseMoved, mouseMoveDatacallbackWin1, id, windowManger::raftelIdToWindowId(win1));
    eventManger::addEvent(events::AppRender, onRenderWin1, -1, windowManger::raftelIdToWindowId(win1));

    eventManger::addEvent(events::MouseMoved, mouseMoveDatacallbackWin2, id, windowManger::raftelIdToWindowId(win2));
    eventManger::addEvent(events::AppRender, onRenderWin2, -1, windowManger::raftelIdToWindowId(win2));

    eventManger::addEvent(events::WindowClose, WindowClose, -1);


    msg = "event added sucssfly";
    logger::LogInfo(msg);

    
    app::run();
    app::close();    
}