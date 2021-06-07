#pragma once
#include <GL/glew.h>

#include "openGLTexture.h"
#include "stb_image.h"
#include "handleOpenGlErrors.h"


namespace LaughTaleEngine
{
    openGLTexture::openGLTexture(const std::string& path)
    {
        stbi_set_flip_vertically_on_load(1);
        uint8_t *buffer = stbi_load(path.c_str(), (int*)&width, (int*)&height, &channels, 4);

        GL_CALL(glGenTextures(1, &id));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
        
        if(buffer)
            stbi_image_free(buffer);
    }

    void openGLTexture::bind(uint32_t slot)
    {
        GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
    }
}