#include "linuxWindow.h"

#include "logger.h"
#include "events.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>


#include <string>

namespace raftelGraphicEngine {
	
	static bool s_GLFWInitialized = false;

	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizeData *eventData = new WindowResizeData(width, height, window);
		eventManger::trigerEvent(events::WindowResize, eventData, window);
		std::string msg = "WindowSizeCallback " + std::to_string(width) + ", " + std::to_string(height);
		logger::LogInfo(msg);
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		IEventData *eventData = new IEventData(events::WindowClose);
		eventData->window = window;
		eventManger::trigerEvent(events::WindowClose, eventData, window);

		std::string msg = "WindowCloseCallback";
		logger::LogInfo(msg);
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyData *eventData = new KeyData(key, scancode, mods, window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::KeyPressed + 
			(action == GLFW_RELEASE) * events::KeyReleased + 
			(action == GLFW_REPEAT) * events::KeyRepeat);

		eventManger::trigerEvent(eventData->eventType, eventData, window);

		std::string msg = "KeyCallback " + std::to_string(eventData->eventType) + ", " + (char)key;
		logger::LogInfo(msg);

	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		mouseClickData *eventData = new mouseClickData(button, mods, window);

		eventData->eventType = (events)(
			(action == GLFW_PRESS) * events::MouseButtonPressed + 
			(action == GLFW_RELEASE) * events::MouseButtonReleased
		);

		eventManger::trigerEvent(eventData->eventType, eventData, window);
		std::string msg = "MouseButtonCallback";
		logger::LogInfo(msg);
	}

	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouseScrollData *eventData = new mouseScrollData(xOffset, yOffset, window);

		eventManger::trigerEvent(events::MouseScrolled, eventData, window);

		std::string msg = "ScrollCallback";
		logger::LogInfo(msg);

	}

	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		mouseMoveData *eventData = new mouseMoveData(xPos, yPos, window);
		eventManger::trigerEvent(events::MouseMoved, eventData, window);

		std::string msg = "CursorPosCallback";
		logger::LogInfo(msg);
	
	}

	void SetCharCallback(GLFWwindow* window, unsigned int keycode)
	{
		keyTypedData *eventData = new keyTypedData(keycode, window);
		eventManger::trigerEvent(events::KeyTyped, eventData, window);

		std::string msg = "SetCharCallback";
		logger::LogInfo(msg);
	}

	void ImGuiMouseButtonPressed(IEntity *eventEntity, IEventData *sendor)
	{
		mouseClickData *eventData = static_cast<mouseClickData *>(sendor);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[eventData->button] = true;
	}

	void ImGuiMouseButtonReleased(IEntity *eventEntity, IEventData *sendor)
	{
		mouseClickData *eventData = static_cast<mouseClickData *>(sendor);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[eventData->button] = false;
	}

	void ImGuiMouseMoved(IEntity *eventEntity, IEventData *sendor)
	{
		mouseMoveData *eventData = static_cast<mouseMoveData *>(sendor);

		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(eventData->xPos, eventData->yPos);
	}

	void ImGuiMouseScrolled(IEntity *eventEntity, IEventData *sendor)
	{
		mouseScrollData *eventData = static_cast<mouseScrollData *>(sendor);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += eventData->xOffset;
		io.MouseWheel += eventData->yOffset;
	}

	void ImGuiWindowResize(IEntity *eventEntity, IEventData *sendor)
	{
		WindowResizeData *eventData = static_cast<WindowResizeData *>(sendor);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2( eventData->windowWidth, eventData->windowHeight);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, eventData->windowWidth, eventData->windowHeight);
	}

	void ImGuiKeyPressed(IEntity *eventEntity, IEventData *sendor)
	{
		KeyData *eventData = static_cast<KeyData *>(sendor);

		std::string msg = "ImGuiKeyPressed";
		logger::LogInfo(msg);

		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[eventData->key] = true;
		
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		 msg = "ImGuiKeyPressedE";
		logger::LogInfo(msg);
	}

	void ImGuiKeyReleased(IEntity *eventEntity, IEventData *sendor)
	{
		KeyData *eventData = static_cast<KeyData *>(sendor);

		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[eventData->key] = false;
	}
	
	void ImGuiKeyTyped(IEntity *eventEntity, IEventData *sendor)
	{
		keyTypedData *eventData = static_cast<keyTypedData *>(sendor);


		ImGuiIO& io = ImGui::GetIO();
		int keycode = eventData->keycode;
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

	}

	void initImGui(GLFWwindow *window)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
		
		eventManger::addEvent(events::MouseButtonPressed, ImGuiMouseButtonPressed, -1, window);
		eventManger::addEvent(events::MouseButtonReleased, ImGuiMouseButtonReleased, -1, window);
		eventManger::addEvent(events::MouseMoved, ImGuiMouseMoved, -1, window);
		eventManger::addEvent(events::MouseScrolled, ImGuiMouseScrolled, -1, window);
		eventManger::addEvent(events::KeyPressed, ImGuiKeyPressed, -1, window);
		eventManger::addEvent(events::KeyRepeat, ImGuiKeyPressed, -1, window);
		eventManger::addEvent(events::KeyReleased, ImGuiKeyReleased, -1, window);
		eventManger::addEvent(events::KeyTyped, ImGuiKeyTyped, -1, window);
		eventManger::addEvent(events::WindowResize, ImGuiMouseButtonPressed, -1, window);

		std::string msg = "imgui init successfully";
		logger::LogInfo(msg);
	}

	GLFWwindow* linuxWindow::Init(linuxWindow data)
	{

		std::string msg = "Creating window " +   data.Title + ", (" +   std::to_string(data.Width) + ", " +   std::to_string(data.Height) + ") ";
		logger::LogInfo(msg);
		

		if (!s_GLFWInitialized)
		{
			glfwSetErrorCallback([](int error, const char* description){ 
				std::string msg =  "glfw error: error code: " + std::to_string(error) + "\t error message" + description;
				logger::LogError(msg);
			});

			int success = glfwInit();

            std::string msg =  "Could not intialize GLFW!" + std::to_string(success);
			logger::condtionLogFatal(msg, success != GLFW_TRUE);

			s_GLFWInitialized = true;
		}

		GLFWwindow* Window = glfwCreateWindow((int)data.Width, (int)data.Height, data.Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		msg = "faild to initalize glad";
		logger::condtionLogFatal(msg, !status);

		glfwSetWindowUserPointer(Window, &data);

		glfwSetWindowSizeCallback(Window, WindowSizeCallback);
		glfwSetWindowCloseCallback(Window, WindowCloseCallback);
		glfwSetKeyCallback(Window, KeyCallback);
		glfwSetMouseButtonCallback(Window, MouseButtonCallback);
		glfwSetScrollCallback(Window, ScrollCallback);
		glfwSetCursorPosCallback(Window, CursorPosCallback);
		glfwSetCharCallback(Window, SetCharCallback);

		initImGui(Window);

		return Window;
	}


	void linuxWindow::Shutdown(GLFWwindow* Window)
	{
		glfwDestroyWindow(Window);
	}

	void onImGuiUpdate(linuxWindow data, onUpdateData *eventData)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(data.Width, data.Height);

		
		
		
		io.DeltaTime = eventData->DeltaTime > 0 ? ((float)eventData->DeltaTime)/1000: 1.0f/60.0f;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void linuxWindow::onUpdate(linuxWindow data, void *sendor)
	{
		onUpdateData *eventData = static_cast<onUpdateData *>(sendor);

		glfwMakeContextCurrent(data.Window);

		IEventData *renderData = new IEventData(events::AppRender);
		eventData->window = data.Window;
		eventManger::trigerEvent(events::AppRender, renderData, data.Window);

		onImGuiUpdate(data, eventData);

		glfwSwapBuffers(data.Window);
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