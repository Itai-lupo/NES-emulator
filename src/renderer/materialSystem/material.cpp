#include "material.h"
#include "openGLTexture.h"

namespace LTE
{
    material::material(const std::string& textureFilePath, glm::vec4 baseColor)
    {
        tex = new openGLTexture(textureFilePath);
        this->baseColor = baseColor;
    }

    material::material(const std::string& textureFilePath)
    {
        tex = new openGLTexture(textureFilePath);
        this->baseColor = {0.0f, 0.0f, 0.0f, 0.0f};
    }

    material::material(glm::vec4 baseColor)
    {
        this->baseColor = baseColor;
    }

    material::~material()
    {
        if(tex != nullptr)
            delete tex;
    }

    void material::bind(shader *s)
    {
        bind({0});
    }

    void material::bind(shader *s, std::vector<uint32_t> textureSlots)
    {
        if(textureSlots.size() == 0) textureSlots.push_back(0);
        tex->bind(textureSlots[0]);
        s->bind();
        s->setUniform1i("texture", textureSlots[0]);
        s->setUniform4f("baseColor", baseColor.r, baseColor.g, baseColor.b, baseColor.a);
    }

    void material::setTexture(texture *tex)
    {
        this->tex = tex;
    }

    void material::setTexture(const std::string& path)
    {
        tex = new openGLTexture(path);
    }

    void material::setBaseColor(glm::vec4 baseColor)
    {
        this->baseColor = baseColor;
    }

    glm::vec4 material::getRGBA()
    {
        return this->baseColor;
    }
}