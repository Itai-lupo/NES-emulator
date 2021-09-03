#include "window.h"
#include "ImGuiEvents.h"
#include "openGLrenderApi.h"
#include "renderer.h"
#include "app.h"

namespace LTE 
{
	void window::init(){
		if(useImGui)
		{
			app::getOsAPI()->makeContextCurrent(id);
			initImGui(id);
		}

		eventManger::addCoustemEventsRoute("App update/winow render/");
		eventManger::addCoustemEventsRoute("Window resize/handel window resize/");
		
		eventManger::startBuildingEvent()->
				setEventRoute("Window resize/handel window resize/" + Title)->
				setEventCallback(onWindowResize)->
				setWindowId(id)->add();

		eventManger::startBuildingEvent()->
				setEventRoute("App update/winow render/" + Title)->
				setEventCallback(onUpdate)->
				setWindowId(id)->add();		
	}

	void window::onUpdate(gameObject *eventEntity, coreEventData *sendor)
	{
		onUpdateData* sendorData = dynamic_cast<onUpdateData*>(sendor);

        app::getOsAPI()->makeContextCurrent(sendorData->windowId);
		sendorData->win->getRenderApi()->SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
		sendorData->win->getRenderApi()->Clear();
		renderer::renderScene(sendorData->win->activeScene, sendorData->win->getRenderApi());
		if(sendorData->win->useImGui){
			onImGuiUpdate(sendorData->win, sendorData);
		}
		sendorData->win->context->SwapBuffers();
		sendorData->win->sceneCollider->checkCollision();
	}

	void window::onWindowResize(gameObject *eventEntity, coreEventData *sendor)
	{
		WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
		LAUGHTALE_ENGINR_LOG_INFO(sendorData->win->Title);
		sendorData->win->Width = sendorData->windowWidth;
		sendorData->win->Height = sendorData->windowHeight;

        app::getOsAPI()->makeContextCurrent(sendorData->windowId);
		sendorData->win->getRenderApi()->SetViewport(0, 0, sendorData->windowWidth, sendorData->windowHeight);
	}

	window::~window()
	{
		delete context;
		delete activeScene;
		
		eventManger::removeEvent("Window resize/handel window resize/" + Title);
		eventManger::removeEvent("App update/winow render/" + Title);
	}

}