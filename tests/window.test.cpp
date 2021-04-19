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

class windowTest
{
    public:
        static void mouseMoveDatacallbackWin1(IEntity *eventEntity, IEventData *sendor)
        {
            mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
            eventEntity->x = sendorData->xPos;
            eventEntity->y = sendorData->yPos;
        }

        static void WindowResize (IEntity *eventEntity, IEventData *sendor)
        {
            WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
            eventEntity->width = sendorData->windowWidth;
            eventEntity->hight = sendorData->windowHeight;
        }

        static void WindowClose(__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
        {
            // app::keepRunning = false;
        }

        static void onRenderWin1(IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
        {
            glClearColor((float)eventEntity->x / eventEntity->width, (float)eventEntity->y / eventEntity->hight, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        static void onRenderWin2(__attribute__((unused)) IEntity *eventEntity,  IEventData *sendor)
        {
	    	// onUpdateData *eventData = static_cast<onUpdateData *>(sendor);

            glClearColor(
                (float)input::GetMouseX(windowManger::raftelIdToWindowReference(sendor->windowId)) / windowManger::getWidth(sendor->windowId), 
                (float)input::GetMouseY(windowManger::raftelIdToWindowReference(sendor->windowId)) / windowManger::getHeight(sendor->windowId), 
                1, 
                1);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        static void ImGuiRender(IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
        {
            ImGuiData *data = static_cast<ImGuiData*>(eventEntity);
            ImGui::Text("%s", data->textToPrint.c_str());
        }

};

TEST(window, openWindowAndUseEvent)
{
    app::init();

    float postions[16] = {
        100.0f, 100.0f,  0.0f, 0.0f,
        150.0f, 100.0f, 1.0f, 0.0f,
        150.0f, 150.0f, 1.0f, 1.0f,
        100.0f, 150.0f, 0.0f, 1.0f
    };

    windowPieceId win1 =  windowManger::addWindow("win 1", true);
    windowPieceId win2 = windowManger::addWindow("win 2");

    OpenGLVertexBuffer *vb = new OpenGLVertexBuffer((const void*)postions, 4 * 4 * sizeof(float));
    vb->pushVertexBufferElement({GL_FLOAT, 2, false, 4});
    vertexBufferId vbId= VertexBufferManger::add(vb);
    LAUGHTALE_ENGINR_LOG_INFO("vb added");
    VertexBufferManger::pushVertexBufferElement(vbId, {GL_FLOAT, 2, false, 4});
    VertexBufferManger::bind(vbId);

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

    eventManger::addEvent(events::ImGuiRender, windowTest::ImGuiRender, imGuiEntityId);

    eventManger::addEvent(events::MouseMoved, windowTest::mouseMoveDatacallbackWin1, id, win1);
    eventManger::addEvent(events::AppRender, windowTest::onRenderWin1, id, win1);
    eventManger::addEvent(events::WindowResize, windowTest::WindowResize, id, win1);

    eventManger::addEvent(events::WindowResize, windowTest::WindowResize, id2, win2);
    eventManger::addEvent(events::AppRender, windowTest::onRenderWin2, id2, win2);

    eventManger::addEvent(events::WindowClose, windowTest::WindowClose);
    
    app::run();
    app::close();    
}