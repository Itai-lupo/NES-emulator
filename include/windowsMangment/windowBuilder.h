#pragma once

#include <string>

#include "core.h"
#include "window.h"

namespace LTE
{
    class windowBuilder
    {
        private:
            window *product;
        public:
            void reset();
            window *build();
            windowBuilder *setTitle(const std::string& title);
            windowBuilder *setWidth(int width);
            windowBuilder *setHeight(int height);
            windowBuilder *useImGui();
            windowBuilder *setActiveScene(scene *activeScene);
    };
}