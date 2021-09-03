#pragma once
#include <string>
#include <vector>
#include <functional>

#include "entity.h"
#include "events.h"
#include "core.h"
#include "window.h"
#include "windowBuilder.h"

namespace LTE
{
    class windowManger
    {
        private:
            static std::vector<window*> windows;
            static inline windowBuilder *buildWindow;
        
            static void onWindowClose(gameObject *eventEntity, coreEventData *sendor);
        public:
            static void init();
            static void close();
            
            static windowPieceId addWindow(std::function<void(windowBuilder *Builder)> buildWindow);
                    
            static window *getWindow(windowPieceId windowId);
    };
}
    