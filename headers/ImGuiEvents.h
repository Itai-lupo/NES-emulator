struct GLFWwindow;
namespace raftelGraphicEngine 
{
    struct IEntity;
    struct IEventData;
    class linuxWindow;
    struct onUpdateData;

    void ImGuiMouseButtonPressed(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);
    void ImGuiMouseButtonReleased(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);
    void ImGuiMouseMoved(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);

    void ImGuiMouseScrolled(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);

    void ImGuiWindowResize(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);

    void ImGuiKeyPressed(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);

    void ImGuiKeyReleased(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);

    void ImGuiKeyTyped(__attribute__((unused)) IEntity *eventEntity, IEventData *sendor);

    void initImGui(GLFWwindow *window);

    void onImGuiUpdate(linuxWindow data, onUpdateData *eventData);
}