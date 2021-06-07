#include "openGLrenderApi.h"
#include "glad/glad.h"
#include "handleOpenGlErrors.h"

namespace LaughTaleEngine
{
    void openGLRenderApi::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void openGLRenderApi::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void openGLRenderApi::DrawIndexed(uint32_t count)
    {
        GL_CALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }

}