#include "jpgFile.h"
#include "windowManger.h"

namespace LTE
{
    jpgFile::jpgFile(const std::string& filePath): file(filePath){}

    jpgFile::~jpgFile()
    {
    }

    asset *jpgFile::genrateAssetForWindow(windowPieceId winId)
    {
        return windowManger::getWindow(winId)->context->getMeshFactory()->createTexture(filePath);
    }

    void jpgFile::loadFileData()
    {

    }
}