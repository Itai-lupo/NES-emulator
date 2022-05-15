#pragma once
#include "core.h"

#include <string>
#include <functional>
#include <vector>
#include <glm/glm.hpp>

namespace LTE {

	class osAPI
	{
		public:
            virtual ~osAPI() = default;

            virtual void pollEvents() = 0;
            virtual bool isWindowOpen(windowPieceId winId) = 0;
            virtual void setVSyncForCurrentContext(bool enabled) = 0;
            virtual void makeContextCurrent(windowPieceId winId) = 0;
            virtual void closeWindow(windowPieceId winId) = 0;
            virtual void swapBuffers(windowPieceId winId) = 0;

	};
}