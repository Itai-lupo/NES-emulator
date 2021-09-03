#pragma once
#include "core.h"
#include "graphicsContext.h"
#include "logger.h"



namespace LTE
{
    class openGLContext : public graphicsContext
	{
        private:
		    windowPieceId windowId;
            renderApi *api;

        public:
            openGLContext(windowPieceId windowId)
                :windowId(windowId)
            {
                
            }

            virtual void Init() override;
            virtual void SwapBuffers() override;
            virtual renderApi *getRenderApi() override;
	};
}
