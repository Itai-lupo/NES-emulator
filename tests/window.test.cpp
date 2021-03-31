
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
    eventCallbackFunc mouseMoveDatacallbackWin1 = [](IEntity *eventEntity, IEventData *sendor)
    {
        mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
        eventEntity->x = sendorData->xPos;
        eventEntity->y = sendorData->yPos;
        
        logger::LogInfo("mouseMoveDatacallbackWin1: " + 
            std::to_string(sendorData->xPos) + 
            ", " + 
            std::to_string(sendorData->yPos));
    };


    eventCallbackFunc mouseMoveDatacallbackWin2 = [](__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
        std::string msg = 
            "mouseMoveDatacallbackWin2: " + 
            std::to_string(sendorData->xPos) + 
            ", " + 
            std::to_string(sendorData->yPos);

        logger::LogInfo(msg);
    };



    eventCallbackFunc Window1Resize = [](IEntity *eventEntity, IEventData *sendor)
    {
        WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
        eventEntity->width = sendorData->windowWidth;
        eventEntity->hight = sendorData->windowHeight;
        std::string msg = 
            "mouseMoveDatacallbackWin1: " + 
            std::to_string(sendorData->windowWidth) + 
            ", " + 
            std::to_string(sendorData->windowHeight);
        logger::LogInfo(msg);
    };


    eventCallbackFunc WindowClose = [](__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        app::keepRunning = false;
    };

    eventCallbackFunc onRenderWin1 = [](IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        std::string msg = 
            "onRenderWin1 mouse pos: " + 
            std::to_string(input::GetMouseX(sendor->window)) + 
            ", " + 
            std::to_string(input::GetMouseX(sendor->window));

        logger::LogInfo(msg);

        glClearColor((float)eventEntity->x / eventEntity->width, (float)eventEntity->y / eventEntity->hight, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    eventCallbackFunc onRenderWin2 = [](__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        glClearColor(0, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    };
    
    
    app::init();

    raftelId win1 =  windowManger::addWindow("win 1");
    raftelId win2 = windowManger::addWindow("win 2", true);

    logger::LogInfo("win1: " + std::to_string(win1) + ", win2: " + std::to_string(win2));
    
    IEntity testEntity;
    testEntity.width = 1280;
    testEntity.hight = 720;
    raftelId id = entityManger::addEntity(&testEntity);
    eventManger::addEvent(events::MouseMoved, mouseMoveDatacallbackWin1, id, windowManger::raftelIdToWindowId(win1));
    // eventManger::addEvent(events::AppRender, onRenderWin1, id, windowManger::raftelIdToWindowId(win1));
    eventManger::addEvent(events::WindowResize, Window1Resize, id, windowManger::raftelIdToWindowId(win1));


    eventManger::addEvent(events::MouseMoved, mouseMoveDatacallbackWin2, id, windowManger::raftelIdToWindowId(win2));
    eventManger::addEvent(events::AppRender, onRenderWin2, -1, windowManger::raftelIdToWindowId(win2));

    eventManger::addEvent(events::WindowClose, WindowClose, -1);


    logger::LogInfo("event added sucssfly");

    
    app::run();
    app::close();    
}