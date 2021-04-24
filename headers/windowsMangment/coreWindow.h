#pragma once
#include <string>
#include <functional>
#include <vector>

enum renderAPI
{
    None = 0, OpenGL = 1
};
typedef u_int64_t windowPieceId;

namespace LaughTaleEngine {

	class coreWindow
	{
		private:
			void *vaManger;
			void *vbManger;
			void *ibManger;
			void *sManger;
			void *Api;
			void *winRenderer;
		public:
			coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui, renderAPI renderAPIType);

			std::string Title;
			unsigned int Width, Height;
			bool useImGui;
			renderAPI renderAPIType;
			windowPieceId id;

			void *getVertexArrayManger(){ return vaManger; };
			void *getVertexBufferManger(){ return vbManger; };
			void *getIndexBufferManger(){ return ibManger; };
			void *getShaderManger(){ return sManger; };
			void *getRenderApi(){ return Api; };
			void *getRenderer(){ return winRenderer; };
			void setRenderApi(void *rApi){ Api = rApi; }
			void setRenderer(void *r){ winRenderer = r; }

	};
}