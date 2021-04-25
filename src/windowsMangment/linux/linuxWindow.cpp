#include "linuxWindow.h"
#include "ImGuiEvents.h"
#include "logger.h"
#include "events.h"
#include <GLFW/glfw3.h>

#include "linuxInput.h"
#include "openGLContext.h"
#include "openGLrenderApi.h"
#include "renderer.h"
#include <string>

namespace LaughTaleEngine {
	
	static bool s_GLFWInitialized = false;

	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizeData *eventData = new WindowResizeData(width, height, (windowPieceId)window);
		
		eventManger::trigerEvent(events::WindowResize, eventData, (windowPieceId)window);
		LAUGHTALE_ENGINR_LOG_INFO(
			"WindowSizeCallback " + 
			std::to_string(width) + ", " + 
			std::to_string(height));
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		IEventData *eventData = new IEventData(events::WindowClose);
		
		eventManger::trigerEvent(events::WindowClose, eventData, (windowPieceId)window);

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

		eventManger::trigerEvent(eventData->eventType, eventData, (windowPieceId)window);

		LAUGHTALE_ENGINR_LOG_INFO(
			"KeyCallback " + 
			std::to_string(eventData->eventType) + ", " + 
			(char)key);
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mouseClickData *eventData = new mouseClickData(button, mods, (windowPieceId)window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::MouseButtonPressed + 
			(action == GLFW_RELEASE) * events::MouseButtonReleased
		);

		eventManger::trigerEvent(eventData->eventType, eventData, (windowPieceId)window);
		LAUGHTALE_ENGINR_LOG_INFO("MouseButtonCallback");
	}

	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScrollData *eventData = new mouseScrollData(xOffset, yOffset, (windowPieceId)window);

		eventManger::trigerEvent(events::MouseScrolled, eventData, (windowPieceId)window);

		LAUGHTALE_ENGINR_LOG_INFO("ScrollCallback");
	}

	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		mouseMoveData *eventData = new mouseMoveData(xPos, yPos, (windowPieceId)window);
		eventManger::trigerEvent(events::MouseMoved, eventData, (windowPieceId)window);

	}

	void SetCharCallback(GLFWwindow* window, unsigned int keycode)
	{
		keyTypedData *eventData = new keyTypedData(keycode, (windowPieceId)window);
		eventManger::trigerEvent(events::KeyTyped, eventData, (windowPieceId)window);

		LAUGHTALE_ENGINR_LOG_INFO("SetCharCallback");
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
		data->setRenderer(new renderer(rApi));
		data->context->Init();
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

		data->winRenderer->beginScene(data->camera);
		glfwMakeContextCurrent(data->Window);


		if(data->useImGui)
			onImGuiUpdate(*data, eventData);

		eventManger::trigerEvent(events::AppRender, eventData, data->id);

		data->winRenderer->endScene();

		data->context->SwapBuffers();
        glfwPollEvents();
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