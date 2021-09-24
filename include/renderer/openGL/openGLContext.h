#pragma once
#include "core.h"
#include "graphicsContext.h"
#include "openGLAbsrtactFactory.h"
#include "logger.h"

#include <thread>


namespace LTE
{
    class openGLContext : public graphicsContext
	{
        private:
		    windowPieceId windowId;
            renderApi *api;
            openGLAbsrtactFactory *meshFactory;
            std::thread *contextThread;
            
            void run();

        public:
            openGLContext(windowPieceId windowId)
                :windowId(windowId)
            {
                meshFactory = new openGLAbsrtactFactory();
            }

            virtual void Init() override;
            virtual void SwapBuffers() override;
            virtual renderApi *getRenderApi() override;
            virtual meshAbsrtactFactory *getMeshFactory() override { return meshFactory; }
	};
}
