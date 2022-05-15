#pragma once
#include "coreCamera.h"
#include "component.h"

namespace LTE
{
    class coreCameraControler: public component
    {
        public:
            virtual ~coreCameraControler() = default;
    
            virtual coreCamera *getCamera() = 0;
            virtual float getAspectRatio() = 0;

            
    };
}