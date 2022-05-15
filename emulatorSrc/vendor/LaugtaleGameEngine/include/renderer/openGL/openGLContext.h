#pragma once
#include "core.h"
#include "graphicsContext.h"
#include "openGLAbsrtactFactory.h"
#include "logger.h"
#include "renderer.h"

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
            renderer *contextRenderEngine;

            bool changeViewPort = false;
            int x, y, width, height;

            void run();

            virtual renderApi *getRenderApi() override;

        public:
            bool windowRun = true;
            openGLContext(windowPieceId windowId)
                :windowId(windowId)
            {
                meshFactory = new openGLAbsrtactFactory();
            }
            ~openGLContext();

            virtual void Init() override;
            virtual void SwapBuffers() override;
            virtual meshAbsrtactFactory *getMeshFactory() override { return meshFactory; }
            virtual void setViewPort(int x, int y, int width, int height) override;
	};
}
