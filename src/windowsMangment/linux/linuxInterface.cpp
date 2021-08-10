#include "logger.h"
#include "events.h"
#include <GLFW/glfw3.h>

#include "linuxInterface.h"
#include "openGLContext.h"
#include <string>

namespace LTE {
	
	bool linuxInterface::s_GLFWInitialized = false;

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
		coreEventData *eventData = new coreEventData();
		eventData->route = "Window close/";
		eventData->windowId = (windowPieceId)window;
		eventManger::trigerEvent(eventData);

		LAUGHTALE_ENGINR_LOG_INFO("WindowCloseCallback");
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyData *eventData = new KeyData(key, scancode, mods, (windowPieceId)window);
 
		eventData->route = 
			action == GLFW_PRESS? "Key pressed/": (action == GLFW_RELEASE? "Key released/":  "Key repeat/");

		eventManger::trigerEvent(eventData);
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mouseClickData *eventData = new mouseClickData(button, mods, (windowPieceId)window);

		eventData->route = 
			action == GLFW_PRESS? "Mouse button pressed/": "Mouse button released/";

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

	linuxInterface::linuxInterface(window *newWindow)
	{
		LAUGHTALE_ENGINR_LOG_INFO(
			"Creating window: " +  
			std::to_string(newWindow->useImGui) + ", " +  
			std::to_string(newWindow->Height) + ", " +   
			std::to_string(newWindow->Width)
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

		windowPtr = glfwCreateWindow((int)newWindow->Width, (int)newWindow->Height, newWindow->Title.c_str(), NULL, NULL);
		newWindow->id = (uint64_t)windowPtr;

		newWindow->context = new openGLContext(windowPtr);
		newWindow->context->Init();
		
		glfwSetWindowUserPointer(windowPtr, newWindow);

		glfwSetWindowSizeCallback(windowPtr, WindowSizeCallback);
		glfwSetWindowCloseCallback(windowPtr, WindowCloseCallback);
		glfwSetKeyCallback(windowPtr, KeyCallback);
		glfwSetMouseButtonCallback(windowPtr, MouseButtonCallback);
		glfwSetScrollCallback(windowPtr, ScrollCallback);
		glfwSetCursorPosCallback(windowPtr, CursorPosCallback);
		glfwSetCharCallback(windowPtr, SetCharCallback);
	}

	
	linuxInterface::~linuxInterface()
	{
		glfwDestroyWindow(windowPtr);
	}

	void linuxInterface::pollEvents()
	{
		glfwPollEvents();
	}

	bool linuxInterface::isOpen()
	{
		return !glfwWindowShouldClose(windowPtr);
	}

	void linuxInterface::setVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
	}

	void linuxInterface::makeContextCurrent()
	{
		glfwMakeContextCurrent(windowPtr);
	}
}