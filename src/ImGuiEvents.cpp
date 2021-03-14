#include "logger.h"
#include "events.h"

#include "ImGuiEvents.h"
#include "imgui.h"
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
        eventManger::addEvent(events::WindowResize, ImGuiWindowResize, -1, window);

        std::string msg = "imgui init successfully";
        logger::LogInfo(msg);
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
}