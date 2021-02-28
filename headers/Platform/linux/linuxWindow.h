#pragma once

#include <string>
#include <functional>
#include <vector>

struct GLFWwindow;
namespace raftelGraphicEngine {

	class linuxWindow
	{
		private:

		public:
			linuxWindow(const std::string& title, unsigned int width, unsigned int height):
				Title(title), Width(width), Height(height){}


			static GLFWwindow* Init(linuxWindow data);
			static void Shutdown(GLFWwindow* Window);
			static void onUpdate(linuxWindow window, void *sendor);
			static bool isOpen(GLFWwindow* Window);
			static void setVSync(bool enabled);
			GLFWwindow* Window;
		
			std::string Title;
			unsigned int Width, Height;

			
	};
}