#pragma once
#include "core.h"
#include "osAPI.h"
#include "window.h"

#include <string>
#include <functional>
#include <vector>
#include <glm/glm.hpp>

#include "soundDevice.h"

namespace LTE {

	class OSAbstractFactory
	{
		public:
            virtual ~OSAbstractFactory() = default;
			virtual windowPieceId createWindow(window *windowToCreate) = 0;
			virtual soundDevice *createInputSoundDevice() = 0;
			virtual soundDevice *createOutputSoundDevice() = 0;
			virtual osAPI *createOsApi() = 0;

			static OSAbstractFactory *init();
	};
}