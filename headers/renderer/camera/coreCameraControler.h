#pragma once
#include "coreCamera.h"
namespace LTE
{
    class coreCameraControler
    {
        public:
            virtual ~coreCameraControler() = default;
    
            virtual coreCamera *getCamera() = 0;
    };
}