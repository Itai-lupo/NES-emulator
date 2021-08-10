#pragma once
#include "graphicsContext.h"
#include "logger.h"

struct GLFWwindow;

namespace LTE
{
    class openGLContext : public graphicsContext
	{
        public:
            openGLContext(GLFWwindow* windowHandle)
                :windowHandle(windowHandle)
            {
                LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("Window handle is null!", windowHandle == nullptr);
            }

            virtual void Init() override;
            virtual void SwapBuffers() override;
        private:
		    GLFWwindow* windowHandle;
	};
}
