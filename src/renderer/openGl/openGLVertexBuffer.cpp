#include "openGLVertexBuffer.h"
#include "glad/glad.h"
#include "handleOpenGlErrors.h"
#include "logger.h"

namespace LaughTaleEngine
{
    vertexBufferId OpenGLVertexBuffer::init()
    {
        RendererID = 0;
        GL_CALL(glGenBuffers(1, &RendererID));
        glBindBuffer(GL_ARRAY_BUFFER, RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        return RendererID;
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &RendererID);
    }

    void OpenGLVertexBuffer::setData(const void *data, uint32_t size)
    {
        this->data = data;
        this->size = size;
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