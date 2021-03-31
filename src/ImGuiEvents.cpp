#include "logger.h"
#include "events.h"

#include "ImGuiEvents.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <functional>

#include <GLFW/glfw3.h>


namespace raftelGraphicEngine 
{
    void ImGuiMouseButtonPressed(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        mouseClickData *eventData = static_cast<mouseClickData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[eventData->button] = true;
    }

    void ImGuiMouseButtonReleased(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        mouseClickData *eventData = static_cast<mouseClickData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[eventData->button] = false;
    }

    void ImGuiMouseMoved(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        mouseMoveData *eventData = static_cast<mouseMoveData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(eventData->xPos, eventData->yPos);
    }

    void ImGuiMouseScrolled(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        mouseScrollData *eventData = static_cast<mouseScrollData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += eventData->xOffset;
        io.MouseWheel += eventData->yOffset;
    }

    void ImGuiWindowResize(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        WindowResizeData *eventData = static_cast<WindowResizeData *>(sendor);
        std::string msg = "ImGuiWindowResize" + std::to_string(eventData->windowWidth) + ", " +  std::to_string(eventData->windowHeight);
        logger::LogInfo(msg);

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2( eventData->windowWidth, eventData->windowHeight);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, eventData->windowWidth, eventData->windowHeight);
    }

    void ImGuiKeyPressed(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
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

    void ImGuiKeyReleased(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        KeyData *eventData = static_cast<KeyData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[eventData->key] = false;
    }

    void ImGuiKeyTyped(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor)
    {
        keyTypedData *eventData = static_cast<keyTypedData *>(sendor);


        ImGuiIO& io = ImGui::GetIO();
        int keycode = eventData->keycode;
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

    }

    void initImGui(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     

        ImGuiStyle& style = ImGui::GetStyle();
		// if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		// {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		// }
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
        
        // eventManger::addEvent(events::MouseButtonPressed, ImGuiMouseButtonPressed, -1, window);
        // eventManger::addEvent(events::MouseButtonReleased, ImGuiMouseButtonReleased, -1, window);
        // eventManger::addEvent(events::MouseMoved, ImGuiMouseMoved, -1, window);
        // eventManger::addEvent(events::MouseScrolled, ImGuiMouseScrolled, -1, window);
        // eventManger::addEvent(events::KeyPressed, ImGuiKeyPressed, -1, window);
        // eventManger::addEvent(events::KeyRepeat, ImGuiKeyPressed, -1, window);
        // eventManger::addEvent(events::KeyReleased, ImGuiKeyReleased, -1, window);
        // eventManger::addEvent(events::KeyTyped, ImGuiKeyTyped, -1, window);
        // eventManger::addEvent(events::WindowResize, ImGuiWindowResize, -1, window);

        std::string msg = "imgui init successfully";
        logger::LogInfo(msg);
    }

    void OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

    void onImGuiUpdate(linuxWindow data, onUpdateData *eventData)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        OnImGuiRender();
        eventManger::trigerEvent(events::ImGuiRender, eventData, data.Window);
		io.DisplaySize = ImVec2(data.Width, data.Height);
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void closeImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
    }
}

