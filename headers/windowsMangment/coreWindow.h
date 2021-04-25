#pragma once
#include <string>
#include <functional>
#include <vector>
#include "core.h"
#include "vertexArrayManger.h"
#include "VertexBufferManger.h"
#include "indexBufferManger.h"
#include "shaderManger.h"
#include "renderApi.h"
#include "renderer.h"
#include "coreCamera.h"

namespace LaughTaleEngine {

	class coreWindow
	{
		protected:
			vertexArrayManger *vaManger;
			VertexBufferManger *vbManger;
			indexBufferManger *ibManger;
			shaderManger *sManger;
			renderApi *Api;
			renderer *winRenderer;
			coreCamera *camera;

		public:
			coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui, renderAPI renderAPIType);

			std::string Title;
			unsigned int Width, Height;
			bool useImGui;
			renderAPI renderAPIType;
			windowPieceId id;

			vertexArrayManger *getVertexArrayManger(){ return vaManger; };
			VertexBufferManger *getVertexBufferManger(){ return vbManger; };
			indexBufferManger *getIndexBufferManger(){ return ibManger; };
			shaderManger *getShaderManger(){ return sManger; };
			renderApi *getRenderApi(){ return Api; };
			renderer *getRenderer(){ return winRenderer; };
			void setRenderApi(renderApi *rApi){ Api = rApi; }
			void setRenderer(renderer *r){ winRenderer = r; }
			void setCamera(coreCamera *cam){ camera = cam; }
	};
}