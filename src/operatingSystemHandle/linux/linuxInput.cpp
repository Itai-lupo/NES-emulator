#include "linuxInput.h"
#include "window.h"
#include "core.h"
#include <GLFW/glfw3.h>

namespace LTE
{
    bool linuxInput::isKeyPressed(int keycode)
    {
        int state = glfwGetKey( windowPtr, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    float linuxInput::GetMouseY()
    {
        double xpos, ypos;
        glfwGetCursorPos(windowPtr, &xpos, &ypos);
        return ypos;
    }

    bool linuxInput::isMouseButtonPressed(int button)
    {
        int state = glfwGetMouseButton( windowPtr, button);

        return state == GLFW_PRESS;
    }

    glm::vec2 linuxInput::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(windowPtr, &xpos, &ypos);
        return { xpos, ypos };
    }

    float linuxInput::GetMouseX()
    {
        double xpos, ypos;
        glfwGetCursorPos(windowPtr, &xpos, &ypos);
        return xpos;
    }
}