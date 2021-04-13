#include "linuxWindow.h"
#include "ImGuiEvents.h"
#include "logger.h"
#include "events.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>


#include <string>

namespace LaughTaleEngine {
	
	static bool s_GLFWInitialized = false;

	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizeData *eventData = new WindowResizeData(width, height, window);
		
		eventManger::trigerEvent(events::WindowResize, eventData, window);
		LAUGHTALE_ENGINR_LOG_INFO(
			"WindowSizeCallback " + 
			std::to_string(width) + ", " + 
			std::to_string(height));
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		IEventData *eventData = new IEventData(events::WindowClose);
		eventData->window = window;
		eventManger::trigerEvent(events::WindowClose, eventData, window);

		LAUGHTALE_ENGINR_LOG_INFO("WindowCloseCallback");
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyData *eventData = new KeyData(key, scancode, mods, window);

		// that a branceles switch 
		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::KeyPressed + 
			(action == GLFW_RELEASE) * events::KeyReleased + 
			(action == GLFW_REPEAT) * events::KeyRepeat);

		eventManger::trigerEvent(eventData->eventType, eventData, window);

		LAUGHTALE_ENGINR_LOG_INFO(
			"KeyCallback " + 
			std::to_string(eventData->eventType) + ", " + 
			(char)key);
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mouseClickData *eventData = new mouseClickData(button, mods, window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::MouseButtonPressed + 
			(action == GLFW_RELEASE) * events::MouseButtonReleased
		);

		eventManger::trigerEvent(eventData->eventType, eventData, window);
		LAUGHTALE_ENGINR_LOG_INFO("MouseButtonCallback");
	}

	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScrollData *eventData = new mouseScrollData(xOffset, yOffset, window);

		eventManger::trigerEvent(events::MouseScrolled, eventData, window);

		LAUGHTALE_ENGINR_LOG_INFO("ScrollCallback");
	}

	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		mouseMoveData *eventData = new mouseMoveData(xPos, yPos, window);
		eventManger::trigerEvent(events::MouseMoved, eventData, window);

		LAUGHTALE_ENGINR_LOG_INFO("CursorPosCallback");	
	}

	void SetCharCallback(GLFWwindow* window, unsigned int keycode)
	{
		keyTypedData *eventData = new keyTypedData(keycode, window);
		eventManger::trigerEvent(events::KeyTyped, eventData, window);

		LAUGHTALE_ENGINR_LOG_INFO("SetCharCallback");
	}

	GLFWwindow* linuxWindow::Init(linuxWindow *data)
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
		glfwMakeContextCurrent(Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LAUGHTALE_ENGINR_CONDTION_LOG_FATAL("faild to initalize glad", !status);

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

		return Window;
	}

	void linuxWindow::Shutdown(GLFWwindow* Window)
	{
		glfwDestroyWindow(Window);
	}

	void linuxWindow::onUpdate(linuxWindow *data, void *sendor)
	{
		onUpdateData *eventData = static_cast<onUpdateData *>(sendor);

		glfwMakeContextCurrent(data->Window);

		IEventData *renderData = new IEventData(events::AppRender);
		eventData->window = data->Window;
		eventManger::trigerEvent(events::AppRender, renderData, data->Window);


		if(data->useImGui)
			onImGuiUpdate(*data, eventData);

		glfwSwapBuffers(data->Window);
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
}