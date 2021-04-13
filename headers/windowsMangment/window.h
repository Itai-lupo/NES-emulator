#pragma once
#include <string>
#include <functional>
#include <vector>

#include "events.h"

#include "core.h"


namespace LaughTaleEngine
{
    class windowManger
    {
        private:
            static bool VSync;
        public:
            static void init();
            static void close();
            
            static windowPieceId addWindow(const std::string& title = "raftel engine", bool useImGui = false, unsigned int width = 1280, unsigned int height = 720);
            static void onUpdate(IEventData *sendor);

            static unsigned int getWidth(windowPieceId windowId);
            static unsigned int getHeight(windowPieceId windowId);

            static windowPtr raftelIdToWindowId(windowPieceId windowId);

            static void setVSync(bool enabled);

            static bool isVSync(){ return VSync; };
    };
}
    
