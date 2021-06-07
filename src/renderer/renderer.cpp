#include "renderer.h"

namespace LaughTaleEngine
{

    void renderer::beginScene(coreCamera *camera)
    {

        api->SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1));
        api->Clear();
        data.camera = camera;
    }

    void renderer::endScene()
    {

    }

    void renderer::Submit(shader *s, uint32_t count, glm::mat4 transform )
    {
        s->bind();
        s->setUniformMat4f("viewProjection", data.camera->getViewProjectionMatrix());
        s->setUniformMat4f("transform", transform);
        api->DrawIndexed(count);
    }


    void renderer::Submit(mesh *toRender )
    {
        shader *s = toRender->getShader();
        if(s == nullptr) return;
        s->bind();
        s->setUniformMat4f("viewProjection", data.camera->getViewProjectionMatrix());
        api->DrawIndexed(toRender->getCount());
    }
}