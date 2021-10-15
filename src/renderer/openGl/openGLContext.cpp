#include "openGLContext.h"
#include "app.h"
#include "openGLrenderApi.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <sys/prctl.h>
#include "windowManger.h"
#include "app.h"
#include "renderer.h"
#include "ImGuiEvents.h"

namespace LTE
{
    void openGLContext::Init()
    {
        app::getOsAPI()->makeContextCurrent(windowId);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("Failed to initialize Glad!", status != 1);

        api = new openGLRenderApi();
        api->init();

        contextThread = new std::thread(&openGLContext::run, this);
    }

    void openGLContext::run()
    {

        std::string thradName = "GL  " + std::to_string(windowId);
        prctl(PR_SET_NAME, thradName.c_str(), 0, 0, 0);

        app::getOsAPI()->makeContextCurrent(windowId);

        uint64_t startTime = app::getTime();
        uint64_t now = app::getTime();

        onUpdateData *sendorData = new onUpdateData(startTime, startTime, 0);

        sendorData->win = windowManger::getWindow(windowId);
        sendorData->windowId = windowId;
        sendorData->route = "window render/" + sendorData->win->Title + "/";

        eventManger::addCoustemEventsRoute("window render/" + sendorData->win->Title + "/");
        
        while (!app::isRuning){}
        
        while (app::keepRunning)
        {
            now = app::getTime();
            sendorData->DeltaTime = now - sendorData->currentTime;
            sendorData->currentTime = now;
            
            meshFactory->build();
            getRenderApi()->SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
            getRenderApi()->Clear();
            renderer::renderScene(sendorData->win->activeScene, sendorData->win->getRenderApi());
            if(sendorData->win->useImGui){
                onImGuiUpdate(sendorData->win, sendorData);
            }
            sendorData->win->context->SwapBuffers();

            eventManger::trigerEvent(sendorData);

    		sendorData->win->sceneCollider->checkCollision();
        }   
    }

    void openGLContext::SwapBuffers()
    {
        app::getOsAPI()->swapBuffers(windowId);
    }

    renderApi *openGLContext::getRenderApi()
    {
        return api;
    }    
} 