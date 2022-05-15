#pragma once
#include "core.h"
#include <string>
#include "graphicsContext.h"
#include "router.h"
#include "asset.h"

namespace LTE
{
    
    class assetManager
    {
        private:
            windowPieceId parentWindowId;
            router<asset*> assetsTree;

        public:
            assetManager(windowPieceId parentWindowId): parentWindowId(parentWindowId){}


            void saveAsset(asset *toSave, const std::string& filePath);
            void loadAssetFromFile(const std::string& filePath);

            template <typename T>
            T *getAsset(const std::string& filePath)
            {
                return static_cast<T*>(getAsset(filePath));
            }

            asset *getAsset(const std::string& filePath);

    };
}