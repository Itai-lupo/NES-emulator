#pragma once
#include "core.h"

namespace LTE
{
    class coreInput
    {
        public:
            virtual bool isKeyPressed(int keycode) = 0;
            virtual bool isMouseButtonPressed(int button) = 0;
            virtual std::pair<float, float> GetMousePosition() = 0;
            virtual float GetMouseX() = 0;
            virtual float GetMouseY() = 0;
    };       
}
