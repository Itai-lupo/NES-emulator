#pragma once
#include "renderApi.h"
#include "meshAbsrtactFactory.h"

namespace LTE
{
    class graphicsContext
	{
        private:
            virtual renderApi *getRenderApi() = 0;

        public:
            virtual void Init() = 0;
            virtual void SwapBuffers() = 0;
	
            virtual meshAbsrtactFactory *getMeshFactory() = 0;
            virtual void setViewPort(int x, int y, int width, int height) = 0;
            

    };
}
