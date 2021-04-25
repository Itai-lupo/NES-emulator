#pragma once
#include "core.h"
#include "coreWindow.h"
#include <string>
#include <functional>
#include <vector>
#include "graphicsContext.h"


struct GLFWwindow;
namespace LaughTaleEngine {

	class linuxWindow: public coreWindow
	{
		private:
		public:
			linuxWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui, renderAPI renderAPIType):
				coreWindow(title, width, height, useImGui, renderAPIType){}


			static void Init(linuxWindow *data);
			static void Shutdown(linuxWindow* Window);
			static void onUpdate(linuxWindow *window, void *sendor);
			static bool isOpen(GLFWwindow* Window);
			static void setVSync(bool enabled);
			static void makeContextCurrent(GLFWwindow* Window);
			GLFWwindow* Window;
			GraphicsContext *context;
	};
}