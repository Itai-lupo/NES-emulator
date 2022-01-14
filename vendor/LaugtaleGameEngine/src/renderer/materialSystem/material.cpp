#include "material.h"
#include "openGLTexture.h"
#include "windowManger.h"

namespace LTE
{
    material::material(const std::string& textureFilePath, glm::vec4 baseColor): texturePath(textureFilePath)
    {
        this->baseColor = baseColor;
    }

    material::material(const std::string& textureFilePath, int tileXIndex, int tileYIndex): 
        texturePath(textureFilePath), tileXIndex(tileXIndex), tileYIndex(tileYIndex)
    {
        this->baseColor = {0.0f, 0.0f, 0.0f, 0.0f};
        
    }

    material::material(const std::string& textureFilePath, int tileIndex):
        texturePath(textureFilePath), tileIndex(tileIndex)
    {
        this->baseColor = {0.0f, 0.0f, 0.0f, 0.0f};

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
        if(texturePath != ""){
            tex = windowManger::getWindow(winId)->assetLibrary->getAsset<texture>(texturePath);
        }
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

    void material::setTileIndex(int tileXIndex, int tileYIndex)
    {
        this->tileXIndex = tileXIndex;
        this->tileYIndex = tileYIndex;
    }

    void material::setTileIndex(int tileIndex)
    {
        this->tileIndex = tileIndex;
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
        if(tex)
            return tex->getId();
        else
            return 0;
    }

    float material::getTexturePostionX(int curnerIndex)
    {
        if(!tex)
            return 0;
            
        if(!tex->useSpriteSheet())
            return texturePostions[curnerIndex].x;

        if(tex->useCustemSpriteSheet())
            return tex->getSpirteStartById(tileIndex).x + texturePostions[curnerIndex].x * tex->getSpirteSizeById(tileIndex).x;
        
        return tex->getSpriteX(tileXIndex) + texturePostions[curnerIndex].x * tex->getSpriteXDelte();
    }

    float material::getTexturePostionY(int curnerIndex)
    {
        if(!tex)
            return 0;
        
        if(!tex->useSpriteSheet())
            return texturePostions[curnerIndex].y;
        
        if(tex->useCustemSpriteSheet())
            return tex->getSpirteStartById(tileIndex).y + texturePostions[curnerIndex].y * tex->getSpirteSizeById(tileIndex).y;
            
        return tex->getSpriteY(tileYIndex) + texturePostions[curnerIndex].y * tex->getSpriteYDelte();
    }

    glm::vec4 material::getRGBA()
    {
        return this->baseColor;
    }
} 