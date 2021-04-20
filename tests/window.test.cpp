#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <string.h>
#include "glad.h"
#include "imgui.h"
#include "handleOpenGlErrors.h"
using namespace LaughTaleEngine;

class ImGuiData: public IEntity
{
    public:
        std::string textToPrint =  "hello world";
};

class renderData: public IEntity
{
    public:
        vertexBufferId vbId;
        indexBufferId ibId;
        vertexArrayId vaId;

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
            app::keepRunning = false;
        }

        static void onRenderWin1(IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
        {
            renderData *renderEntity = static_cast<renderData *>(eventEntity);

            glClearColor((float)eventEntity->x / eventEntity->width, (float)eventEntity->y / eventEntity->hight, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            VertexBufferManger::bind(renderEntity->vbId);
            GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));

        }

        static void onRenderWin2(IEntity *eventEntity,  IEventData *sendor)
        {
            renderData *renderEntity = static_cast<renderData *>(eventEntity);
            glClearColor(
                (float)input::GetMouseX(windowManger::raftelIdToWindowReference(sendor->windowId)) / windowManger::getWidth(sendor->windowId), 
                (float)input::GetMouseY(windowManger::raftelIdToWindowReference(sendor->windowId)) / windowManger::getHeight(sendor->windowId), 
                1, 
                1);
            glClear(GL_COLOR_BUFFER_BIT);
            

            indexBufferManger::bind(renderEntity->ibId);
            VertexBufferManger::bind(renderEntity->vbId);

            GL_CALL(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr));
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

    float postions[6] = { 
        0.5f,  -0.5f,
        -0.5f,   0.0f,
        0.5f,   0.5f
    };

    float postions2[12] = { 
         0.0f,    1.0f, 
         0.5f,    0.5f,
         0.5f,   -0.5f,
         0.0f,   -1.0f,
        -0.5f,   -0.5f,
        -0.5f,    0.5f
    };

    unsigned int indices[12] = {
         0, 1, 5,
         1, 2, 4,
         2, 3, 4,
         1, 4, 5
     };

    windowPieceId win1 =  windowManger::addWindow("win 1", true);
    windowPieceId win2 = windowManger::addWindow("win 2");

    renderData *win1RenderData = new renderData();
    renderData *win2RenderData = new renderData();
    
    windowManger::bindContext(win1);
    OpenGLVertexBuffer *vb = new OpenGLVertexBuffer(postions, 2 * 3 * sizeof(float));
    win1RenderData->vbId = VertexBufferManger::add(vb);


    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, false, 2*sizeof(float), 0));

    windowManger::bindContext(win2);
    OpenGLVertexBuffer *vb2 = new OpenGLVertexBuffer(postions2, 3 * 6 * sizeof(float));
    win2RenderData->vbId = VertexBufferManger::add(vb2);
    
    indexBuffer *ib = new openGLIndexBuffer(indices, 12);
    win2RenderData->ibId = indexBufferManger::add(ib);

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, false, 2*sizeof(float), 0));

    win1RenderData->width = 1280;
    win1RenderData->hight = 720;

    win2RenderData->width = 1280;
    win2RenderData->hight = 720;

    ImGuiData *imGuiData = new ImGuiData();
    
    entityTaleId id = entityManger::addEntity(win1RenderData);
    entityTaleId id2 = entityManger::addEntity(win2RenderData);
    entityTaleId imGuiEntityId = entityManger::addEntity(imGuiData);

    eventManger::addEvent(events::ImGuiRender, windowTest::ImGuiRender, imGuiEntityId);

    eventManger::addEvent(events::MouseMoved, windowTest::mouseMoveDatacallbackWin1, id, win1);
    eventManger::addEvent(events::AppRender, windowTest::onRenderWin1, id, win1);
    eventManger::addEvent(events::WindowResize, windowTest::WindowResize, id, win1);

    eventManger::addEvent(events::WindowResize, windowTest::WindowResize, id2, win2);
    eventManger::addEvent(events::AppRender, windowTest::onRenderWin2, id2, win2);

    eventManger::addEvent(events::WindowClose, windowTest::WindowClose);
            LAUGHTALE_ENGINR_LOG_INFO("a");
    
    app::run();
    app::close();    
}