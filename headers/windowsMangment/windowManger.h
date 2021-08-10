#pragma once
#include <string>
#include <vector>

#include "entity.h"
#include "events.h"
#include "core.h"
#include "window.h"
#include "coreWindowFactory.h"

namespace LTE
{
    class windowManger
    {
        private:
            friend class coreWindowFactory;
            static windowPieceId addWindow(window *newWindow);
            static std::vector<window*> windows;
            static inline coreWindowFactory *windowFactory;
        
            static void onWindowClose(gameObject *eventEntity, coreEventData *sendor);
        public:
            static void init();
            static void close();
            
                    
            static window *getWindow(windowPieceId windowId);
            static coreWindowFactory *startBuildingWindow(){ return windowFactory->startBuilding(); }
    };
}
    