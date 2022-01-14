#pragma once
#include <utility>

#include "coreInput.h"

struct GLFWwindow;
namespace LTE
{
    class linuxInput: public coreInput
    {
        private:
            GLFWwindow *windowPtr;
        public:
            linuxInput(GLFWwindow *windowPtr): windowPtr(windowPtr){}
            bool isKeyPressed(int keycode);
            bool isMouseButtonPressed(int button);
            glm::vec2 GetMousePosition();
            float GetMouseX();
            float GetMouseY();
    };       
}
