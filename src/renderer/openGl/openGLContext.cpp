#include "openGLContext.h"
#include "app.h"
#include "openGLrenderApi.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace LTE
{
    void openGLContext::Init()
    {
        app::getOsAPI()->makeContextCurrent(windowId);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("Failed to initialize Glad!", status != 1);

        api = new openGLRenderApi();
        api->init();
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

