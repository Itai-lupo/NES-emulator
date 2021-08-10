#include "window.h"
#include "ImGuiEvents.h"
#include "openGLrenderApi.h"
#include "renderer.h"

namespace LTE {
	void window::onUpdate(gameObject *eventEntity, coreEventData *sendor)
	{
		onUpdateData* sendorData = dynamic_cast<onUpdateData*>(sendor);

		sendorData->win->operatingSystemPipLine->makeContextCurrent();
		sendorData->win->renderPipLine->SetClearColor({0.0f, 0.0f, 1.0f, 1.0f});
		sendorData->win->renderPipLine->Clear();
		renderer::renderScene(sendorData->win->activeScene, sendorData->win->renderPipLine);
		if(sendorData->win->useImGui){
			onImGuiUpdate(sendorData->win, sendorData);
		}
		sendorData->win->context->SwapBuffers();
		sendorData->win->sceneCollider->checkCollision();
		sendorData->win->operatingSystemPipLine->pollEvents();
	}

	void window::onWindowResize(gameObject *eventEntity, coreEventData *sendor)
	{
		WindowResizeData* sendorData = dynamic_cast<WindowResizeData*>(sendor);
		LAUGHTALE_ENGINR_LOG_INFO(sendorData->win->Title);
		sendorData->win->Width = sendorData->windowWidth;
		sendorData->win->Height = sendorData->windowHeight;

		sendorData->win->operatingSystemPipLine->makeContextCurrent();
		sendorData->win->renderPipLine->SetViewport(0, 0, sendorData->windowWidth, sendorData->windowHeight);
	}

	void window::init(){
		if(useImGui)
		{
			operatingSystemPipLine->makeContextCurrent();
			initImGui(operatingSystemPipLine);
		}
		LAUGHTALE_ENGINR_LOG_INFO(Title);

		eventManger::addCoustemEventsRoute("App update/winow render/");
		eventManger::addCoustemEventsRoute("Window resize/handel window resize/");
		
		eventManger::startBuildingEvent()->
				setEventRoute("Window resize/handel window resize/" + Title)->
				setEventCallback(onWindowResize)->
				setWindowId(id)->add();

		LAUGHTALE_ENGINR_LOG_INFO("App update/winow render/" + Title)
		eventManger::startBuildingEvent()->
				setEventRoute("App update/winow render/" + Title)->
				setEventCallback(onUpdate)->
				setWindowId(id)->add();
	}

	window::~window()
	{
		delete operatingSystemPipLine;
		delete context;
		delete activeScene;
		delete renderPipLine;
		
		eventManger::removeEvent("Window resize/handel window resize " + Title);
		eventManger::removeEvent("App update/winow render " + Title);
	}

}