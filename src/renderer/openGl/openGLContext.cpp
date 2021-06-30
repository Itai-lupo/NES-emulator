#include "openGLContext.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace LTE
{
    void openGLContext::Init()
    {
        glfwMakeContextCurrent(windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LAUGHTALE_ENGINR_CONDTION_LOG_INFO("Failed to initialize Glad!", status != 1);
    }

    void openGLContext::SwapBuffers()
    {
        glfwSwapBuffers(windowHandle);
    }
    
} 

