#include "openGLIndexBuffer.h"
#include "handleOpenGlErrors.h"
#include "glad/glad.h"

namespace LTE
{
    indexBufferId openGLIndexBuffer::init()
    {
        GL_CALL(glGenBuffers(1, &RendererID));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), ib, GL_STATIC_DRAW));
        return RendererID;
    }

    openGLIndexBuffer::~openGLIndexBuffer()
    {
        GL_CALL(glDeleteBuffers(1, &RendererID));
    }

    void openGLIndexBuffer::bind()
    {
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID));
    }

    void openGLIndexBuffer::unbind()
    {
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));    
    }
}