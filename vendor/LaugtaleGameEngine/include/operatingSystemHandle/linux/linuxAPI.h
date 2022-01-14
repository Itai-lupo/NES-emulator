#pragma once
#include "core.h"
#include "osAPI.h"


namespace LTE 
{

	class linuxAPI: public osAPI
      {
		public:
            virtual ~linuxAPI() = default;

            virtual void pollEvents() override;
            virtual bool isWindowOpen(windowPieceId winId) override;
            virtual void setVSyncForCurrentContext(bool enabled) override;
            virtual void makeContextCurrent(windowPieceId winId) override;
            virtual void closeWindow(windowPieceId winId) override;
            virtual void swapBuffers(windowPieceId winId) override;
	};
}