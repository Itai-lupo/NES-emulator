#ifdef __linux__

#include "logger.h"
#include "events.h"
#include <GLFW/glfw3.h>

#include "OSAbstractFactory.h"
#include "linuxAbstractFactory.h"
#include "linuxAPI.h"
#include "window.h"
#include <string>

namespace LTE 
{


	OSAbstractFactory *OSAbstractFactory::init()
	{
		return new linuxAbstractFactory();
	}

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

	linuxAbstractFactory::linuxAbstractFactory()
	{
		
		int success = glfwInit();

		LAUGHTALE_ENGINR_CONDTION_LOG_FATAL(
			"Could not intialize GLFW!" + std::to_string(success),
				success != GLFW_TRUE);

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
	}

	
	linuxAbstractFactory::~linuxAbstractFactory()
	{
	}

	osAPI *linuxAbstractFactory::createOsApi()
	{
		return new linuxAPI();
	}

	windowPieceId linuxAbstractFactory::createWindow(window *windowToCreate)
	{
		GLFWwindow *winRef = glfwCreateWindow(windowToCreate->Width, windowToCreate->Height, windowToCreate->Title.c_str(), NULL, NULL);

		glfwSetWindowSizeCallback(winRef, WindowSizeCallback);
		glfwSetWindowCloseCallback(winRef, WindowCloseCallback);
		glfwSetKeyCallback(winRef, KeyCallback);
		glfwSetMouseButtonCallback(winRef, MouseButtonCallback);
		glfwSetScrollCallback(winRef, ScrollCallback);
		glfwSetCursorPosCallback(winRef, CursorPosCallback);
		glfwSetCharCallback(winRef, SetCharCallback);

		windowToCreate->id = (windowPieceId)winRef;
		return windowToCreate->id;
	}
}

#endif