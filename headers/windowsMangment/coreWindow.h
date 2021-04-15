
#include <string>
#include <functional>
#include <vector>

typedef u_int64_t windowPieceId;

namespace LaughTaleEngine {

	class coreWindow
	{
		private:

		public:
			coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui):
				Title(title), Width(width), Height(height), useImGui(useImGui){}
		
			std::string Title;
			unsigned int Width, Height;
			bool useImGui;
			windowPieceId id;
	};
}