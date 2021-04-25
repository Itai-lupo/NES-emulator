#include "renderer.h"

namespace LaughTaleEngine
{

    void renderer::beginScene(coreCamera *camera)
    {
        data.camera = camera;
    }

    void renderer::endScene()
    {

    }

    void renderer::Submit(shader *s, uint32_t count)
    {
        s->bind();
        s->setUniformMat4f("viewProjection", data.camera->getViewProjectionMatrix());
        api->DrawIndexed(count);
    }

}