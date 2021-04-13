
#include <string>
#include <functional>
#include <vector>

typedef u_int32_t windowPieceId;

namespace LaughTaleEngine {

	class coreWindow
	{
		private:

		public:
			coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui, windowPieceId id):
				Title(title), Width(width), Height(height), useImGui(useImGui), id(id){}
		
			std::string Title;
			unsigned int Width, Height;
			bool useImGui;
			windowPieceId id;
	};
}