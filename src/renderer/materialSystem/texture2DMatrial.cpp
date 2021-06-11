#include "texture2DMatrial.h"
#include "openGLTexture.h"

namespace LaughTaleEngine
{
    texture2DMatrial::texture2DMatrial(const std::string& textureFilePath)
    {
        tex = new openGLTexture(textureFilePath);
    }

    texture2DMatrial::~texture2DMatrial()
    {
        free(tex);
    }

    void texture2DMatrial::bind(shader *s)
    {
        tex->bind(slot);
        s->bind();
        s->setUniform1i("texture", slot);
    }

}