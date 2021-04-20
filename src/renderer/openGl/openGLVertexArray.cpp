#include "handleOpenGlErrors.h"
#include "openGLVertexArray.h"
#include "glad/glad.h"

#include "logger.h"
#include <string>

namespace LaughTaleEngine
{
    openGLVertexArray::~openGLVertexArray()
    {
        GL_CALL(glDeleteVertexArrays(1, &rendererId));
    }

    vertexArrayId openGLVertexArray::init()
    {
        GL_CALL(glGenVertexArrays(1, &rendererId));
        return rendererId;
    }

    void openGLVertexArray::AddBuffer(VertexBuffer& vb)
    {
        bind();
        const auto& elements = vb.getElements();

        vb.bind();
        unsigned long i = 0, offset = 0;
        for(const auto& element : elements)
        {
            LAUGHTALE_ENGINR_LOG_INFO("AddBuffer" + std::to_string(i) + ", " + std::to_string(element.count) + ", " + std::to_string(vb.GetStride()));
            GL_CALL(glEnableVertexAttribArray(i));
            GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized,  vb.GetStride(), (const void*)offset));
            offset += element.count * element.sizeOfType;
            i++;
        }
    }

    void openGLVertexArray::bind()
    {
        GL_CALL(glBindVertexArray(rendererId));
    }

    void openGLVertexArray::unbind()
    {
        GL_CALL(glBindVertexArray(0));
    }
}

