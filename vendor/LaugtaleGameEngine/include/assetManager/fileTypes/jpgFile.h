#pragma once
#include "file.h"
#include "texture.h"

namespace LTE
{
    class jpgFile: public file
    {
        private:
        public:
            jpgFile(const std::string& filePath);

            virtual ~jpgFile();

            virtual asset *genrateAssetForWindow(windowPieceId);
            virtual void loadFileData();

    }; 
}