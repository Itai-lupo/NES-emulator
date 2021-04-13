#pragma once
#include <utility>

struct GLFWwindow;
namespace LaughTaleEngine
{
    class linuxInput
    {
        typedef int raftelId;
        public:
            static bool isKeyPressed(int keycode, raftelId windowId);
            static bool isMouseButtonPressed(int button, raftelId windowId);
            static std::pair<float, float> GetMousePosition(raftelId windowId);
            static float GetMouseX(raftelId windowId);
            static float GetMouseY(raftelId windowId);

            static bool isKeyPressed(int keycode, GLFWwindow* windowId);
            static bool isMouseButtonPressed(int button, GLFWwindow* windowId);
            static std::pair<float, float> GetMousePosition(GLFWwindow* windowId);
            static float GetMouseX(GLFWwindow* windowId);
            static float GetMouseY(GLFWwindow* windowId);
    };       
}
