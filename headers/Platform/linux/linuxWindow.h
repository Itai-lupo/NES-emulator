#pragma once

#include <string>
#include <functional>
#include <vector>


#include <GLFW/glfw3.h>

namespace raftelGraphicEngine {

	class linuxWindow
	{
		private:

		public:
			linuxWindow(const std::string& title, unsigned int width, unsigned int height):
				Title(title), Width(width), Height(height){}


			static GLFWwindow* Init(linuxWindow data);
			static void Shutdown(GLFWwindow* Window);
			static void onUpdate(linuxWindow window);
			static bool isOpen(GLFWwindow* Window);

			GLFWwindow* Window;
		
			std::string Title;
			unsigned int Width, Height;

			
	};
}