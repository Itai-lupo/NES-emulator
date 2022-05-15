#pragma once
#include "core.h"
#include "asset.h"
#include <string>

namespace LTE
{
    class file
    {
        protected:
            std::string filePath;
        public:
            virtual ~file() = default;
            file(const std::string& filePath): filePath(filePath){}

            virtual asset *genrateAssetForWindow(windowPieceId  winId) = 0;
            virtual void loadFileData() = 0;

            std::string getFilePath(){ return filePath; }
    }; 
}