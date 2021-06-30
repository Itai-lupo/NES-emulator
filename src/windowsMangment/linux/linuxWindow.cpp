#include "linuxWindow.h"
#include "ImGuiEvents.h"
#include "logger.h"
#include "events.h"
#include <GLFW/glfw3.h>

#include "linuxInput.h"
#include "openGLContext.h"
#include "openGLrenderApi.h"
#include "renderer.h"
#include "orthographicCameraControler.h"
#include <string>

namespace LTE {
	
	static bool s_GLFWInitialized = false;

	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizeData *eventData = new WindowResizeData(width, height, (windowPieceId)window);
		
		eventManger::trigerEvent(eventData);
		LAUGHTALE_ENGINR_LOG_INFO(
			"WindowSizeCallback " + 
			std::to_string(width) + ", " + 
			std::to_string(height));
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		coreEventData *eventData = new coreEventData(events::WindowClose);
		eventData->eventType = events::WindowClose;
		eventData->windowId = (windowPieceId)window;
		eventManger::trigerEvent(eventData);

		LAUGHTALE_ENGINR_LOG_INFO("WindowCloseCallback");
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyData *eventData = new KeyData(key, scancode, mods, (windowPieceId)window);

		// that a branceles switch 
		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::KeyPressed + 
			(action == GLFW_RELEASE) * events::KeyReleased + 
			(action == GLFW_REPEAT) * events::KeyRepeat);

		eventManger::trigerEvent(eventData);
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mouseClickData *eventData = new mouseClickData(button, mods, (windowPieceId)window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::MouseButtonPressed + 
			(action == GLFW_RELEASE) * events::MouseButtonReleased
		);

		eventManger::trigerEvent(eventData);
		LAUGHTALE_ENGINR_LOG_INFO("MouseButtonCallback");
	}

	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScrollData *eventData = new mouseScrollData(xOffset, yOffset, (windowPieceId)window);

		eventManger::trigerEvent(eventData);

		LAUGHTALE_ENGINR_LOG_INFO("ScrollCallback");
	}

	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		mouseMoveData *eventData = new mouseMoveData(xPos, yPos, (windowPieceId)window);
		eventManger::trigerEvent(eventData);

	}

	void SetCharCallback(GLFWwindow* window, unsigned int keycode)
	{
		keyTypedData *eventData = new keyTypedData(keycode, (windowPieceId)window);
		eventManger::trigerEvent(eventData);
	}

	void linuxWindow::Init(linuxWindow *data)
	{
		LAUGHTALE_ENGINR_LOG_INFO(
			"Creating window: " +  
			std::to_string(data->useImGui) + ", " +  
			std::to_string(data->Height) + ", " +   
			std::to_string(data->Width)
		);

		if (!s_GLFWInitialized)
		{
			glfwSetErrorCallback(
				[](int error, const char* description)
				{
					LAUGHTALE_ENGINR_LOG_ERROR(
						"glfw error: \nerror code: " + 
						std::to_string(error) + 
						",\t error message" + 
						description
						);
				}
			);

			int success = glfwInit();

			LAUGHTALE_ENGINR_CONDTION_LOG_FATAL(
				"Could not intialize GLFW!" + std::to_string(success),
				 success != GLFW_TRUE);

			s_GLFWInitialized = true;
		}

		GLFWwindow* Window = glfwCreateWindow((int)data->Width, (int)data->Height, data->Title.c_str(), NULL, NULL);
		data->id = (uint64_t)Window;
		data->Window = Window;

		renderApi *rApi;
		switch (data->renderAPIType)
		{
			case renderAPI::OpenGL:
				data->context = new openGLContext(Window);
				rApi = new openGLRenderApi();
				break;
			
			default:
				LAUGHTALE_ENGINR_LOG_FATAL("can't open window, no render Type specifiction");
		}
		
		data->setRenderApi(rApi);
		data->context->Init();
		data->setRenderer(new renderer(rApi));
		glfwSetWindowUserPointer(Window, data);
		
		
		glfwSetWindowSizeCallback(Window, WindowSizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);
		glfwSetKeyCallback(Window, KeyCallback);
		glfwSetMouseButtonCallback(Window, MouseButtonCallback);
		glfwSetScrollCallback(Window, ScrollCallback);
		glfwSetCursorPosCallback(Window, CursorPosCallback);
		glfwSetCharCallback(Window, SetCharCallback);

		if(data->useImGui)
			initImGui(Window);

        data->cameraControler = new orthographicCameraControler((float)data->Width/(float)data->Height, data->id);

	}

	void linuxWindow::Shutdown(linuxWindow* Window)
	{
		if(Window->useImGui)
			closeImGui();
		Window->useImGui = false;
		glfwDestroyWindow(Window->Window);
	}

	void linuxWindow::onUpdate(linuxWindow *data, void *sendor)
	{
		onUpdateData *eventData = static_cast<onUpdateData *>(sendor);
		eventData->eventType = events::AppRender;
		eventData->windowId = data->id;
		makeContextCurrent(data->Window);
		data->winRenderer->beginScene(data->getCamera());

		eventManger::trigerEvent(eventData);
		
		if(data->useImGui)
			onImGuiUpdate(*data, eventData);

		data->winRenderer->endScene();

		data->context->SwapBuffers();
        glfwPollEvents();
		eventData->eventType = events::AppUpdate;
	}

	bool linuxWindow::isOpen(GLFWwindow* window)
	{
		return !glfwWindowShouldClose(window);
	}

	void linuxWindow::setVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
	}

	void linuxWindow::makeContextCurrent(GLFWwindow* Window)
	{
		glfwMakeContextCurrent(Window);
	}
}