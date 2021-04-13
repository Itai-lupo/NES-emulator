
#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <string.h>
#include "glad.h"
#include "imgui.h"

using namespace LaughTaleEngine;

class ImGuiData: public IEntity
{
    public:
        std::string textToPrint =  "hello world";
};


TEST(window, openWindowAndUseEvent)
{
    eventCallbackFunc mouseMoveDatacallbackWin1 = [](IEntity *eventEntity, IEventData *sendor)
    {
        mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
        eventEntity->x = sendorData->xPos;
        eventEntity->y = sendorData->yPos;
    };


    eventCallbackFunc mouseMoveDatacallbackWin2 = [](__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        // mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
    };

    eventCallbackFunc WindowResize = [](IEntity *eventEntity, IEventData *sendor)
    {
        WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
        eventEntity->width = sendorData->windowWidth;
        eventEntity->hight = sendorData->windowHeight;
    };

    eventCallbackFunc WindowClose = [](__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        app::keepRunning = false;
    };

    eventCallbackFunc onRenderWin1 = [](IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        glClearColor((float)eventEntity->x / eventEntity->width, (float)eventEntity->y / eventEntity->hight, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    eventCallbackFunc onRenderWin2 = [](IEntity *eventEntity,  IEventData *sendor)
    {
        glClearColor((float)input::GetMouseX(sendor->window) / eventEntity->width, (float)input::GetMouseY(sendor->window) / eventEntity->hight, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    };
    
    eventCallbackFunc ImGuiRender = [](IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
    {
        ImGuiData *data = static_cast<ImGuiData*>(eventEntity);
        ImGui::Text("%s", data->textToPrint.c_str());
    };

    app::init();

    windowPieceId win1 =  windowManger::addWindow("win 1");
    windowPieceId win2 = windowManger::addWindow("win 2", true);

    IEntity testEntity;
    testEntity.width = 1280;
    testEntity.hight = 720;

    IEntity testEntity2;
    testEntity2.width = 1280;
    testEntity2.hight = 720;

    ImGuiData *imGuiData = new ImGuiData();
    
    entityTaleId id = entityManger::addEntity(&testEntity);
    entityTaleId id2 = entityManger::addEntity(&testEntity2);
    entityTaleId imGuiEntityId = entityManger::addEntity(imGuiData);

    eventManger::addEvent(events::ImGuiRender, ImGuiRender, imGuiEntityId);

    eventManger::addEvent(events::MouseMoved, mouseMoveDatacallbackWin1, id, windowManger::raftelIdToWindowId(win1));
    eventManger::addEvent(events::AppRender, onRenderWin1, id, windowManger::raftelIdToWindowId(win1));
    eventManger::addEvent(events::WindowResize, WindowResize, id, windowManger::raftelIdToWindowId(win1));

    eventManger::addEvent(events::MouseMoved, mouseMoveDatacallbackWin2, id2, windowManger::raftelIdToWindowId(win2));
    eventManger::addEvent(events::WindowResize, WindowResize, id2, windowManger::raftelIdToWindowId(win2));
    eventManger::addEvent(events::AppRender, onRenderWin2, id2, windowManger::raftelIdToWindowId(win2));

    eventManger::addEvent(events::WindowClose, WindowClose, -1);


    LAUGHTALE_ENGINR_LOG_INFO("event added sucssfly");

    
    app::run();
    app::close();    
}