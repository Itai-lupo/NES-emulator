#pragma once
#include "glad/glad.h"

#include "openGLTexture.h"
#include "stb_image.h"
#include "handleOpenGlErrors.h"


namespace LTE
{
    openGLTexture::openGLTexture(const std::string& path): texture(path)
    {
        
    }

    openGLTexture::~openGLTexture()
    {
        glDeleteTextures(1, &id);
    }

    void openGLTexture::init()
    {
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
			
		if (data)
		{
			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 2)
			{
				internalFormat = GL_RG8;
				dataFormat = GL_RG;
			}

			else if (channels == 1)
			{
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &id);
			if (channels == 2) {
				GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
				glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            }
			glTextureStorage2D(id, 1, internalFormat, width, height);

			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			LAUGHTALE_ENGINR_LOG_ERROR("failed to load texture frome file:"  << path << "\t because: " << stbi_failure_reason() );
		}
    }


    void openGLTexture::bind(int slot)
    {
		glBindTextureUnit(slot, id);
    }
    
    void openGLTexture::unbind()
    {
        GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }
}