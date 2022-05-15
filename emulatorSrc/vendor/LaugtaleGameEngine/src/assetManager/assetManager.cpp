#include "assetManager.h"
#include "filesManager.h"

namespace LTE
{
    void assetManager::saveAsset(asset *toSave, const std::string& filePath)
    {
        assetsTree.addRoute(filePath);
        assetsTree.addValue(filePath, toSave);
    }   

    void assetManager::loadAssetFromFile(const std::string& filePath)
    {
        if(assetsTree[filePath])
            return;
        file* f = fileManager::getFile(filePath);
        if(!f)
            return;
        asset *fileAsset = f->genrateAssetForWindow(parentWindowId);
        saveAsset(fileAsset, filePath);
    }

    asset *assetManager::getAsset(const std::string& filePath)
    {
        if(assetsTree[filePath])
            return assetsTree[filePath];
        
        loadAssetFromFile(filePath);
        return assetsTree[filePath];

    }

}