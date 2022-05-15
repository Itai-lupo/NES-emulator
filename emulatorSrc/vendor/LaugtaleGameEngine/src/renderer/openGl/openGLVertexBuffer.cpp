#include "openGLVertexBuffer.h"
#include "glad/glad.h"
#include "handleOpenGlErrors.h"
#include "logger.h"

namespace LTE
{
    void OpenGLVertexBuffer::init()
    {
        GL_CALL(glGenBuffers(1, &RendererID));
        glBindBuffer(GL_ARRAY_BUFFER, RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &RendererID);
    }

    void OpenGLVertexBuffer::setData(const void *data, uint32_t size)
    {
        this->data = data;
        this->size = size;
        
        glBindBuffer(GL_ARRAY_BUFFER, RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, RendererID);
    }

    void OpenGLVertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}