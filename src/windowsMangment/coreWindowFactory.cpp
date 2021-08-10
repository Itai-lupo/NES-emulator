#include <string>

#include "coreWindowFactory.h"
#include "windowManger.h"

namespace LTE
{
    coreWindowFactory *coreWindowFactory::startBuilding()
    {
        prodect  = new window();

        return this;
    }

    coreWindowFactory *coreWindowFactory::setWindowName(const std::string& name)
    {
        prodect->Title = name;
        return this;
    }

    coreWindowFactory *coreWindowFactory::setWindowWidth(int width)
    {
        prodect->Width = width;
        return this;
    }

    coreWindowFactory *coreWindowFactory::setWindowHeight(int height)
    {
        prodect->Height = height;
        return this;
    }

    coreWindowFactory *coreWindowFactory::useImGui()
    {
        prodect->useImGui = true;
        return this;
    }

    coreWindowFactory *coreWindowFactory::setRenderApiType(renderAPI renderAPIType)
    {
        return this;
    }

    coreWindowFactory *coreWindowFactory::setCameraType(cameraControler type)
    {
        return this;
    }

    windowPieceId coreWindowFactory::add()
    {
        build();
        prodect->init();
        return windowManger::addWindow(prodect);
    }     
}