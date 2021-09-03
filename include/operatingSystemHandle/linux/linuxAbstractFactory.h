#pragma once
#include "core.h"
#include "OSAbstractFactory.h"


#include <string>
#include <functional>
#include <vector>


struct GLFWwindow;
namespace LTE {

	class linuxAbstractFactory: public OSAbstractFactory
	{
		public:
			linuxAbstractFactory();
			~linuxAbstractFactory();

			virtual osAPI *createOsApi() override;
			virtual windowPieceId createWindow(window *windowToCreate) override;
	};
}