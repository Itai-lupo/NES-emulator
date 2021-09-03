#include "core.h"
#include "window.h"
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


namespace LTE 
{
    void ImGuiMouseButtonPressed(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        mouseClickData *eventData = static_cast<mouseClickData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[eventData->button] = true;
    }

    void ImGuiMouseButtonReleased(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        mouseClickData *eventData = static_cast<mouseClickData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[eventData->button] = false;
    }

    void ImGuiMouseMoved(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        mouseMoveData *eventData = static_cast<mouseMoveData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(eventData->xPos, eventData->yPos);
    }

    void ImGuiMouseScrolled(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        mouseScrollData *eventData = static_cast<mouseScrollData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += eventData->xOffset;
        io.MouseWheel += eventData->yOffset;
    }

    void ImGuiWindowResize(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        WindowResizeData *eventData = static_cast<WindowResizeData *>(sendor);
        LAUGHTALE_ENGINR_LOG_INFO(
            "ImGuiWindowResize" + 
            std::to_string(eventData->windowWidth) + ", " +  
            std::to_string(eventData->windowHeight));

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2( eventData->windowWidth, eventData->windowHeight);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, eventData->windowWidth, eventData->windowHeight);
    }

    void ImGuiKeyPressed(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        KeyData *eventData = static_cast<KeyData *>(sendor);

        LAUGHTALE_ENGINR_LOG_INFO("ImGuiKeyPressed");

        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[eventData->key] = true;
        
        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        LAUGHTALE_ENGINR_LOG_INFO("ImGuiKeyPressed");
    }

    void ImGuiKeyReleased(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        KeyData *eventData = static_cast<KeyData *>(sendor);

        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[eventData->key] = false;
    }

    void ImGuiKeyTyped(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor)
    {
        keyTypedData *eventData = static_cast<keyTypedData *>(sendor);


        ImGuiIO& io = ImGui::GetIO();
        int keycode = eventData->keycode;
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

    }

    void initImGui(windowPieceId windowId)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)windowId, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        LAUGHTALE_ENGINR_LOG_INFO("imgui init successfully");
    }

    void onImGuiUpdate(window *data, onUpdateData *eventData)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        std::string temp = eventData->route;
        eventData->route = "ImGui render/";
        eventManger::trigerEvent(eventData);
        eventData->route = temp;
        
		io.DisplaySize = ImVec2(data->Width, data->Height);
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