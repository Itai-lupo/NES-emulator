#include "pngFile.h"
#include "windowManger.h"

namespace LTE
{
    pngFile::pngFile(const std::string& filePath): file(filePath){}

    pngFile::~pngFile()
    {
    }

    asset *pngFile::genrateAssetForWindow(windowPieceId winId)
    {
        return windowManger::getWindow(winId)->context->getMeshFactory()->createTexture(filePath);
    }

    void pngFile::loadFileData()
    {

    }
}