#pragma once

#include <string>

#include "core.h"
#include "window.h"

namespace LTE
{
    class coreWindowFactory
    {
        protected:
            window *prodect;
            virtual void build() = 0;
            
        public:
            coreWindowFactory *startBuilding();
            coreWindowFactory *setWindowName(const std::string& name);
            coreWindowFactory *setWindowWidth(int width);
            coreWindowFactory *setWindowHeight(int height);
            coreWindowFactory *useImGui();
            coreWindowFactory *setRenderApiType(renderAPI renderAPIType);
            coreWindowFactory *setCameraType(cameraControler type);

            windowPieceId add();
            
    };
}