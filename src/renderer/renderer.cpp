#include "renderer.h"
#include "mesh.h"
#include "meshRenderer.h"


namespace LTE
{

    void renderer::renderScene(scene *Scene, renderApi *renderPipLine)
    {
        renderPipLine->SetClearColor(Scene->backgroundColor->getRGBA());
        renderPipLine->Clear();

        renderer::Scene = Scene;
        renderer::renderPipLine = renderPipLine;
        
        const glm::mat4& ViewProjectionMatrix = Scene->camera->getComponent<coreCameraControler>()->getCamera()->getViewProjectionMatrix();
        for(gameObject *toRender: *Scene->objects)
        {
            toRender->getComponent<meshRenderer>()->render(renderPipLine, ViewProjectionMatrix);
        }
    }
}