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
        
        
        for(gameObject *toRender: *Scene->objects)
        {
            toRender->getComponent<meshRenderer>()->render(renderPipLine, Scene->camera->getComponent<coreCameraControler>()->getCamera()->getViewProjectionMatrix());
        }
    }


    // void renderer::Submit(shader *s, uint32_t count, glm::mat4 transform )
    // {
    //     s->bind();
    //     LAUGHTALE_ENGINR_LOG_INFO("A");
    //     s->setUniformMat4f("viewProjection", data.camera->getViewProjectionMatrix());
    //     s->setUniformMat4f("transform", transform);
    //     LAUGHTALE_ENGINR_LOG_INFO("A");
    //     renderPipLine->DrawIndexed(count);
    // }


    // void renderer::Submit(mesh *toRender )
    // {
    //     toRender->bind({0});
    //     LAUGHTALE_ENGINR_LOG_INFO("A");

    //     toRender->getShader()->setUniformMat4f("viewProjection", Scene->camera->getViewProjectionMatrix());
    //     LAUGHTALE_ENGINR_LOG_INFO("A");

    //     renderPipLine->DrawIndexed(toRender->getCount());
    // }
}