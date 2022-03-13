#pragma once
#include "glad/glad.h"

#include "openGLCustomTexture.h"
#include "stb_image.h"
#include "handleOpenGlErrors.h"

#include "logger.h"

namespace LTE
{
  openGLCustomTexture::openGLCustomTexture(const std::pair<int, int>& size): 
  size(size), data(new RGB[size.first * size.second])
  {
    for (size_t i = 0; i <= size.first * size.second; i++)
    {
      data[i] = {0};
    }
  }

  openGLCustomTexture::~openGLCustomTexture()
  {
      glDeleteTextures(1, &id);
  }

  void openGLCustomTexture::init()
  {
    GLenum internalFormat = 0, dataFormat = 0;
    internalFormat = GL_RGB8;
    dataFormat = GL_RGB;
    

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, internalFormat, size.first, size.second);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }


  void openGLCustomTexture::bind(int slot)
  {
    glTextureSubImage2D(id, 0, 0, 0, size.first, size.second, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTextureUnit(slot, id);
  }
  
  void openGLCustomTexture::unbind()
  {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
  }

  void openGLCustomTexture::setRGBValue(const std::pair<int, int>& cords, const uint8_t rgb[3])
  {
    if(cords.first >= size.first || cords.second >= size.second)
    {
      LAUGHTALE_ENGINR_LOG_ERROR("out of bond(" << (int)cords.first << ", " << (int)cords.second << ")")
      return;
    }


    data[(size.second - cords.second - 1) * size.first + cords.first].R = rgb[0];
    data[(size.second - cords.second - 1) * size.first + cords.first].G = rgb[1];
    data[(size.second - cords.second - 1) * size.first + cords.first].B = rgb[2];
    // LAUGHTALE_ENGINR_LOG_INFO(cords.second * size.first + cords.first << ", " << cords.first << ", " << cords.second)
    // LAUGHTALE_ENGINR_LOG_INFO((int)data[cords.second * size.first + cords.first].R << ", " <<(int)data[cords.second * size.first + cords.first].G << ", " <<(int)data[cords.second * size.first + cords.first].B << ", ")
  }
}