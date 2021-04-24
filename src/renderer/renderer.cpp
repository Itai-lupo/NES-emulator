#include "renderer.h"


namespace LaughTaleEngine
{
    void renderer::BeginScene()
    {

    }

    void renderer::EndScene()
    {

    }

    void renderer::Submit(uint32_t count)
    {
        api->DrawIndexed(count);
    }

}