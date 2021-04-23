#pragma once
#include <string>
#include <functional>
#include <vector>


typedef u_int64_t windowPieceId;

namespace LaughTaleEngine {

	class coreWindow
	{
		private:
			void *vaManger;
			void *vbManger;
			void *ibManger;
			void *sManger;
		public:
			coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui);

			std::string Title;
			unsigned int Width, Height;
			bool useImGui;
			windowPieceId id;
			void *getVertexArrayManger(){ return vaManger; };
			void *getVertexBufferManger(){ return vbManger; };
			void *getIndexBufferManger(){ return ibManger; };
			void *getShaderManger(){ return sManger; };

	};
}