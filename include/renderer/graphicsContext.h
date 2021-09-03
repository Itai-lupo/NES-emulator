#pragma once
#include "renderApi.h"

namespace LTE
{
    class graphicsContext
	{
        public:
            virtual void Init() = 0;
            virtual void SwapBuffers() = 0;
	
            virtual renderApi *getRenderApi() = 0;

    };
}
