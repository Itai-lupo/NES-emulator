#pragma once
#include "core.h"
#include "window.h"
#include "operatingSystemInterface.h"
#include "graphicsContext.h"

#include <string>
#include <functional>
#include <vector>


struct GLFWwindow;
namespace LTE {

	class linuxInterface: public operatingSystemInterface
	{
		private:
			GLFWwindow* windowPtr;
			graphicsContext *context;
			static bool s_GLFWInitialized;

		public:
			linuxInterface(window *newWindow);
			~linuxInterface();

			
			virtual void pollEvents() override;
			virtual bool isOpen() override;
			virtual void setVSync(bool enabled) override;
			virtual void makeContextCurrent() override;	

			virtual void *getWindowRef() override { return windowPtr; };

	};
}