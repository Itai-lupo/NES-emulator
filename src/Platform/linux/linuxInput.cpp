#include "linuxInput.h"
#include "window.h"
#include "core.h"
#include <GLFW/glfw3.h>

namespace raftelGraphicEngine
{
    bool linuxInput::isKeyPressed(int keycode, raftelId windowId)
    {
        windowPtr win = windowManger::raftelIdToWindowId(windowId);
        return isKeyPressed(keycode, win);
    }

    bool linuxInput::isMouseButtonPressed(int button, raftelId windowId)
    {
        windowPtr win = windowManger::raftelIdToWindowId(windowId);
        return isMouseButtonPressed(button, win);
    }

    std::pair<float, float> linuxInput::GetMousePosition(raftelId windowId)
    {
        windowPtr win = windowManger::raftelIdToWindowId(windowId);
        return GetMousePosition(win);
    }

    float linuxInput::GetMouseX(raftelId windowId)
    {
        windowPtr win = windowManger::raftelIdToWindowId(windowId);
        return GetMouseX(win);
    }

    float linuxInput::GetMouseY(raftelId windowId)
    {
        windowPtr win = windowManger::raftelIdToWindowId(windowId);
        return GetMouseY(win);
    }

    bool linuxInput::isKeyPressed(int keycode, windowPtr windowId)
    {
        int state = glfwGetKey( windowId, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool linuxInput::isMouseButtonPressed(int button, windowPtr windowId)
    {
        int state = glfwGetMouseButton( windowId, button);

        return state == GLFW_PRESS;
    }

    std::pair<float, float> linuxInput::GetMousePosition(windowPtr windowId)
    {
        double xpos, ypos;
        glfwGetCursorPos(windowId, &xpos, &ypos);
        return { xpos, ypos };
    }

    float linuxInput::GetMouseX(windowPtr windowId)
    {
        double xpos, ypos;
        glfwGetCursorPos(windowId, &xpos, &ypos);
        return xpos;
    }
    
    float linuxInput::GetMouseY(windowPtr windowId)
    {
        double xpos, ypos;
        glfwGetCursorPos(windowId, &xpos, &ypos);
        return ypos;
    }
}