struct GLFWwindow;
namespace LTE 
{
    struct IEntity;
    struct coreEventData;
    class linuxWindow;
    struct onUpdateData;

    void ImGuiMouseButtonPressed(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);
    void ImGuiMouseButtonReleased(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);
    void ImGuiMouseMoved(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);

    void ImGuiMouseScrolled(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);

    void ImGuiWindowResize(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);

    void ImGuiKeyPressed(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);

    void ImGuiKeyReleased(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);

    void ImGuiKeyTyped(__attribute__((unused)) IEntity *eventEntity, coreEventData *sendor);

    void initImGui(GLFWwindow *window);
    void closeImGui();

    void onImGuiUpdate(linuxWindow data, onUpdateData *eventData);
}