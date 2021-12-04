#include "material.h"
#include "openGLTexture.h"
#include "windowManger.h"

namespace LTE
{
    material::material(const std::string& textureFilePath, glm::vec4 baseColor): texturePath(textureFilePath)
    {
        this->baseColor = baseColor;
    }

    material::material(const std::string& textureFilePath): texturePath(textureFilePath)
    {
        this->baseColor = {0.0f, 0.0f, 0.0f, 0.0f};
    }

    material::material(glm::vec4 baseColor): texturePath("")
    {
        this->baseColor = baseColor;
    }

    void material::init(gameObject *parent)
    {
        if(texturePath != "")
            tex = windowManger::getWindow(winId)->assetLibrary->getAsset<texture>(texturePath);
    }


    void material::end()
    {

    }

    void material::bind()
    {
        bind({0});
    }

    void material::bind(std::vector<uint32_t> textureSlots)
    {
        if(textureSlots.size() == 0) textureSlots.push_back(0);
        if(tex != nullptr)
        {
            tex->bind(textureSlots[0]);
        }
    }

    void material::setTexture(texture *tex)
    {
        this->tex = tex;
    }

    void material::setTexture(const std::string& path)
    {
        texturePath = path;
        this->tex = windowManger::getWindow(winId)->assetLibrary->getAsset<texture>(path);
    }

    void material::setBaseColor(glm::vec4 baseColor)
    {
        this->baseColor = baseColor;
    }

    std::string material::getTextureName()
    {
        return texturePath;
    }

    texture *material::getTexture()
    {
        return tex;
    }


    textureId material::getTextureId()
    {
        return tex->getId();
    }



    glm::vec4 material::getRGBA()
    {
        return this->baseColor;
    }
} 