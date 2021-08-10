#pragma once
#include <string>
#include <functional>
#include <vector>


#include "core.h"
#include "operatingSystemInterface.h"
#include "graphicsContext.h"
#include "scene.h"
#include "renderApi.h"

#include "events.h"
#include "entity.h"

namespace LTE {
    class coreWindowFactory;
	class window
	{
		private:
			static inline void onUpdate(gameObject *eventEntity, coreEventData *sendor);
			static inline void onWindowResize(gameObject *eventEntity, coreEventData *sendor);

		public:
			operatingSystemInterface *operatingSystemPipLine;
			graphicsContext *context; 
			scene *activeScene;
			renderApi *renderPipLine;
            colliderSystem2D *sceneCollider;


			std::string Title = "laughtale new window";
			unsigned int Width = 1280, Height = 720;
			bool useImGui = false;
			windowPieceId id;
			void init();
			~window();
	};
}