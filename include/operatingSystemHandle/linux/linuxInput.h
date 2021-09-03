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
            bool isKeyPressed(int keycode);
            bool isMouseButtonPressed(int button);
            std::pair<float, float> GetMousePosition();
            float GetMouseX();
            float GetMouseY();
    };       
}
