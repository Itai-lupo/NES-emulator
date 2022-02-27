#include "window.h"
#include "ImGuiEvents.h"
#include "openGLrenderApi.h"
#include "renderer.h"
#include "app.h"

namespace LTE 
{
	void window::init(){
        context->Init();
		if(useImGui)
		{
			app::getOsAPI()->makeContextCurrent(id);
			initImGui(id);
		}

		eventManger::addCoustemEventsRoute("Window resize/handel window resize/");
		
		eventManger::startBuildingEvent()->
				setEventRoute("Window resize/handel window resize/" + Title)->
				setEventCallback(onWindowResize)->
				setWindowId(id)->add();

		
	}

	void window::onWindowResize(gameObject *eventEntity, coreEventData *sendor)
	{
		WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);

		sendorData->win->Width = sendorData->windowWidth;
		sendorData->win->Height = sendorData->windowHeight;
		sendorData->win->context->setViewPort(0, 0, sendorData->windowWidth, sendorData->windowHeight);
	}

	window::~window()
	{
		delete context;
		delete activeScene;
		
		eventManger::removeEvent("Window resize/handel window resize/" + Title);
		eventManger::removeEvent("App update/winow render/" + Title);
	}

}