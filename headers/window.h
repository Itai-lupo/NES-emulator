#pragma once
#include <string>
#include <functional>
#include <vector>

#include "events.h"

#include "core.h"


namespace raftelGraphicEngine
{
    class windowManger
    {
        private:
            static bool VSync;
        public:
            static void init();
            static void close();
            
            static raftelId addWindow(const std::string& title = "raftel engine", unsigned int width = 1280, unsigned int height = 720);
            static void onUpdate(IEventData *sendor);

            static unsigned int getWidth(raftelId windowId);
            static unsigned int getHeight(raftelId windowId);

            static windowPtr raftelIdToWindowId(raftelId windowId);

            static void setVSync(bool enabled);

            static bool isVSync(){ return VSync; };
    };
}