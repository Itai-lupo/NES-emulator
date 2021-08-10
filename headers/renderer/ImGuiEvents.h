#pragma once

struct GLFWwindow;
namespace LTE 
{
    struct gameObject;
    struct coreEventData;
    class window;
    struct onUpdateData;

    void ImGuiMouseButtonPressed(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);
    void ImGuiMouseButtonReleased(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);
    void ImGuiMouseMoved(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);

    void ImGuiMouseScrolled(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);

    void ImGuiWindowResize(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);

    void ImGuiKeyPressed(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);

    void ImGuiKeyReleased(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);

    void ImGuiKeyTyped(__attribute__((unused)) gameObject *eventEntity, coreEventData *sendor);

    void initImGui(operatingSystemInterface *operatingSystemPipLine);
    void closeImGui();

    void onImGuiUpdate(window *data, onUpdateData *eventData);
}