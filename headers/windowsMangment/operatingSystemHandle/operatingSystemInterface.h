#pragma once
#include "core.h"

#include <string>
#include <functional>
#include <vector>

namespace LTE {

	class operatingSystemInterface
	{
		public:
            virtual ~operatingSystemInterface() = default;
			virtual void pollEvents() = 0;
			virtual bool isOpen() = 0;
			virtual void setVSync(bool enabled) = 0;
			virtual void makeContextCurrent() = 0;	
			virtual void *getWindowRef() = 0;
	};
}