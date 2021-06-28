#pragma once
#include "coreCamera.h"
namespace LaughTaleEngine
{
    class coreCameraControler
    {
        public:
            virtual ~coreCameraControler() = default;
    
            virtual coreCamera *getCamera() = 0;
    };
}