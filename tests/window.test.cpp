#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <string.h>
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>


using namespace LaughTaleEngine;

class ImGuiData: public IEntity
{
    public:
        std::string textToPrint =  "hello world";
};

class renderData: public IEntity
{
    public:
        int width, hight; 
        vertexBufferId vbId;
        indexBufferId ibId;
        vertexArrayId vaId;
        vertexBufferId vbId2;
        indexBufferId ibId2;
        vertexArrayId vaId2;
        shaderId sId;
        float deg = 0.0;
        float x = 0.0, y = 0.0;
};


class windowTest
{
    public:
        static void mouseMoveDatacallbackWin1(IEntity *eventEntity, IEventData *sendor)
        {
            mouseMoveData* sendorData = dynamic_cast<mouseMoveData*>(sendor);
            renderData *renderEntity = static_cast<renderData *>(eventEntity);

            renderEntity->x = sendorData->xPos;
            renderEntity->y = sendorData->yPos;
        }

        static void WindowResize (IEntity *eventEntity, IEventData *sendor)
        {
            WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
            renderData *renderEntity = static_cast<renderData *>(eventEntity);

            renderEntity->width = sendorData->windowWidth;
            renderEntity->hight = sendorData->windowHeight;
        }

        static void WindowClose(__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
        {
            app::keepRunning = false;
        }

        static void onRenderWin1(IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
        {
            
            renderData *renderEntity = static_cast<renderData *>(eventEntity);
            renderApi *api = windowManger::getRenderApi(sendor->windowId);
            renderer *r = windowManger::getRenderer(sendor->windowId);

            float mouseX = (float)input::GetMouseX(windowManger::raftelIdToWindowReference(sendor->windowId)) / windowManger::getWidth(sendor->windowId);
            float mouseY = (float)input::GetMouseY(windowManger::raftelIdToWindowReference(sendor->windowId)) / windowManger::getHeight(sendor->windowId);
            
            api->SetClearColor(glm::vec4((float)renderEntity->x / renderEntity->width, (float)renderEntity->y / renderEntity->hight, 0, 1));
            api->Clear();

            
            windowManger::bindVA(sendor->windowId, renderEntity->vaId2);
            windowManger::bindS(sendor->windowId, renderEntity->sId);
            windowManger::bindIB(sendor->windowId, renderEntity->ibId2);
            
            shaderManger *windowShaderManger = windowManger::getShaderManger(sendor->windowId);
            shader *s = windowShaderManger->getShader(renderEntity->sId);
            s->setUniform4f("colorOffset", mouseX, 1 - mouseY, 0.0f, 1.0f);
            orthographicCameraControler *camera = new orthographicCameraControler(1.6f/0.9f, sendor->windowId);
            camera->getCamera()->SetPosition({ -(mouseX * 3.2 - 1.6), (mouseY * 1.8 - 0.9), 1.0f});

            camera->getCamera()->setRotation(renderEntity->deg);
            windowManger::setCamera(sendor->windowId, camera);

            r->Submit(s, 12);
        }

        static void onRenderWin2(IEntity *eventEntity,  IEventData *sendor)
        {
            renderData *renderEntity = static_cast<renderData *>(eventEntity);
            renderApi *api = windowManger::getRenderApi(sendor->windowId);


            float mouseX = 
                (float)input::GetMouseX(windowManger::raftelIdToWindowReference(sendor->windowId)) / 
                windowManger::getWidth(sendor->windowId);
            float mouseY = 
                (float)input::GetMouseY(windowManger::raftelIdToWindowReference(sendor->windowId)) / 
                windowManger::getHeight(sendor->windowId);


            api->SetClearColor(glm::vec4(mouseX, mouseY, 1, 1));
            api->Clear();

            renderer *r = windowManger::getRenderer(sendor->windowId);
            shaderManger *windowShaderManger = windowManger::getShaderManger(sendor->windowId);
            shader *s = windowShaderManger->getShader(renderEntity->sId);
            windowManger::bindVA(sendor->windowId, renderEntity->vaId);
            windowManger::bindIB(sendor->windowId, renderEntity->ibId);
            s->setUniform4f("colorOffset", 1.0f, 1.0f, 0.0f, 1.0f);
            static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
            for(int x = -10; x < 10; x++)
            {
                for(int y = -10; y < 10; y++)
                {
                    glm::vec3 pos(x * 0.101f, y * 0.101f, 0.0f);
                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                    r->Submit(s, windowManger::getIndexBufferCount(sendor->windowId, renderEntity->ibId), transform);
                }
            }

            windowManger::bindVA(sendor->windowId, renderEntity->vaId2);
            windowManger::bindS(sendor->windowId, renderEntity->sId);
            windowManger::bindIB(sendor->windowId, renderEntity->ibId2);
            
            s->setUniform4f("colorOffset", mouseX, 1 - mouseY, 0.0f, 1.0f);
            
            orthographicCameraControler *camera = new orthographicCameraControler(1.6f / 0.9f, sendor->windowId);            
            camera->getCamera()->setRotation(renderEntity->deg);
            windowManger::setCamera(sendor->windowId, camera);

            r->Submit(s, windowManger::getIndexBufferCount(sendor->windowId, renderEntity->ibId2), glm::translate(glm::mat4(1.0f), { (mouseX * 3.2 - 1.6), -(mouseY * 1.8 - 0.9), 1.0f}) * scale);

        }

        static void ImGuiRender(IEntity *eventEntity, IEventData *sendor)
        {
            onUpdateData *e = static_cast<onUpdateData *>(sendor);
            ImGuiData *data = static_cast<ImGuiData*>(eventEntity);
            ImGui::Text("%d", e->DeltaTime);
        }


        static void onKeyPress(IEntity *eventEntity, IEventData *sendor)
        {
            renderData *renderEntity = static_cast<renderData *>(eventEntity);
            KeyData *e = static_cast<KeyData *>(sendor);

            renderEntity->deg += ((char)e->key == 'Q') - ((char)e->key == 'E');
            renderEntity->x   += (float)(((char)e->key == 'D') - ((char)e->key == 'A')) / 100;
            renderEntity->y   += (float)(((char)e->key == 'S') - ((char)e->key == 'W')) / 100;
        }
};

TEST(window, openWindowAndUseEvent)
{
    return;
    app::init();

    float postions[8] = { 
         -0.5f,   -0.5f,
         0.5f,    -0.5f,
         0.5f,     0.5f,
         -0.5f,    0.5f
        };

    unsigned int indices[6] = {
         0, 1, 2,
         0, 3, 2,
     };

    float postions2[12] = { 
         0.0f,    0.9f,
         0.8f,    0.45f,
         0.8f,   -0.45f,
         0.0f,   -0.9f,
        -0.8f,   -0.45f,
        -0.8f,    0.45f
        };

    unsigned int indices2[12] = {
         0, 1, 5,
         1, 2, 4,
         2, 3, 4,
         1, 4, 5
     };

    windowPieceId win1 =  windowManger::addWindow("win 1", true);
    windowPieceId win2 = windowManger::addWindow("win 2");

    windowManger::setCamera(win1, new orthographicCameraControler(1.6f / 0.9f, win1));

    windowManger::setCamera(win2, new orthographicCameraControler(1.6f / 0.9f, win2));

    renderData *win1RenderData = new renderData();
    renderData *win2RenderData = new renderData();
    
    windowManger::bindContext(win1);


    OpenGLVertexBuffer *vbs = new OpenGLVertexBuffer(postions, 2 * 6 * sizeof(float));
    OpenGLVertexBuffer *vbs2 = new OpenGLVertexBuffer(postions, 2 * 6 * sizeof(float));

    win1RenderData->vbId = windowManger::add(win1, vbs);
    win2RenderData->vbId = windowManger::add(win2, vbs2);

    windowManger::pushElement(win1, win1RenderData->vbId, {LT_FLOAT, 2, false, 4});
    windowManger::pushElement(win2, win2RenderData->vbId, {LT_FLOAT, 2, false, 4});

    vertexArray *vas = new openGLVertexArray();
    win1RenderData->vaId = windowManger::add(win1, vas);

    vertexArray *va2s = new openGLVertexArray();
    win2RenderData->vaId = windowManger::add(win2, va2s);


    OpenGLVertexBuffer *vb = new OpenGLVertexBuffer(postions2, 2 * 6 * sizeof(float));
    OpenGLVertexBuffer *vb2 = new OpenGLVertexBuffer(postions2, 2 * 6 * sizeof(float));

    win1RenderData->vbId2 = windowManger::add(win1, vb);
    win2RenderData->vbId2 = windowManger::add(win2, vb2);

    windowManger::pushElement(win1, win1RenderData->vbId2, {LT_FLOAT, 2, false, 4});
    windowManger::pushElement(win2, win2RenderData->vbId2, {LT_FLOAT, 2, false, 4});



    vertexArray *va = new openGLVertexArray();
    win1RenderData->vaId2 = windowManger::add(win1, va);

    vertexArray *va2 = new openGLVertexArray();
    win2RenderData->vaId2 = windowManger::add(win2, va2);


    indexBuffer *ib = new openGLIndexBuffer(indices, 6);
    win1RenderData->ibId = windowManger::add(win1, ib);
    win2RenderData->ibId = windowManger::add(win2, ib);

    indexBuffer *ib2 = new openGLIndexBuffer(indices2, 12);
    win1RenderData->ibId2 = windowManger::add(win1, ib2);
    win2RenderData->ibId2 = windowManger::add(win2, ib2);

    shader *s = new openGLShader("res/shaders/Basic.shader");
    win1RenderData->sId = windowManger::add(win1, s);
    win2RenderData->sId = windowManger::add(win2, s);
    

    windowManger::addBuffer(win1, win1RenderData->vaId , win1RenderData->vbId);
    windowManger::addBuffer(win1, win1RenderData->vaId2, win1RenderData->vbId2);
    windowManger::addBuffer(win2, win2RenderData->vaId , win2RenderData->vbId);
    windowManger::addBuffer(win2, win2RenderData->vaId2, win2RenderData->vbId2);
  
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

    eventManger::addEvent(events::KeyPressed, windowTest::onKeyPress, id);
    eventManger::addEvent(events::KeyPressed, windowTest::onKeyPress, id2);
    eventManger::addEvent(events::KeyRepeat, windowTest::onKeyPress, id);
    eventManger::addEvent(events::KeyRepeat, windowTest::onKeyPress, id2);


    eventManger::addEvent(events::WindowClose, windowTest::WindowClose);
    
    app::run();
    app::close();    
}