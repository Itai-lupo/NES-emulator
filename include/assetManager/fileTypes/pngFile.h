#pragma once
#include "file.h"
#include "texture.h"

namespace LTE
{
    class pngFile: public file
    {
        private:
            texture *tex;
        public:
            pngFile(const std::string& filePath);

            virtual ~pngFile();

            virtual asset *genrateAssetForWindow(windowPieceId);
            virtual void loadFileData();

    }; 
}