
#include <string>
#include <functional>
#include <vector>

// struct windowPtr;
namespace raftelGraphicEngine {

	class coreWindow
	{
		private:

		public:
			coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui):
				Title(title), Width(width), Height(height), useImGui(useImGui){}


			// static  windowPtr* Init(coreWindow data);
			// static void Shutdown(windowPtr* Window);
			// static void onUpdate(coreWindow window, void *sendor);
			// static bool isOpen(windowPtr* Window);
			// static void setVSync(bool enabled);
			// windowPtr* Window;
		
			std::string Title;
			unsigned int Width, Height;
			bool useImGui;
			
	};
}