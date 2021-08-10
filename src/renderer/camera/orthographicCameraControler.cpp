#include "orthographicCameraControler.h"
#include "logger.h"
#include "entity.h"

namespace LTE
{
    void orthographicCameraControler::OnMouseScrolled(gameObject *cameraInfo, coreEventData *sendor)
    {
        mouseScrollData *eventData = dynamic_cast<mouseScrollData *>(sendor);
        orthographicCameraControler *thisCamera = cameraInfo->getComponent<orthographicCameraControler>();

        thisCamera->zoomLevel -= eventData->yOffset * 0.25f;
		thisCamera->zoomLevel = std::max(thisCamera->zoomLevel, 0.25f);

		thisCamera->camera.SetProjection(-thisCamera->aspectRatio * thisCamera->zoomLevel, thisCamera->aspectRatio * thisCamera->zoomLevel, -thisCamera->zoomLevel, thisCamera->zoomLevel);
    }

    void orthographicCameraControler::OnWindowResized(gameObject *cameraInfo, coreEventData *sendor)
    {
        WindowResizeData *eventData = dynamic_cast<WindowResizeData *>(sendor);
        orthographicCameraControler *thisCamera = cameraInfo->getComponent<orthographicCameraControler>();

        thisCamera->aspectRatio = (float)eventData->windowWidth / (float)eventData->windowHeight;
		thisCamera->camera.SetProjection(-thisCamera->aspectRatio * thisCamera->zoomLevel, thisCamera->aspectRatio * thisCamera->zoomLevel, -thisCamera->zoomLevel, thisCamera->zoomLevel);
    }

    void orthographicCameraControler::OnUpdate(gameObject *cameraInfo, coreEventData *sendor)
    {
        onUpdateData *eventData = dynamic_cast<onUpdateData *>(sendor);   
    }

    orthographicCameraControler::orthographicCameraControler(float aspectRatio, bool useCameraRotation)
        :aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), useCameraRotation(useCameraRotation)
    {

    }

    orthographicCameraControler::~orthographicCameraControler()
    {

    }

    void orthographicCameraControler::init(gameObject *parent)
    {
        OnUpdateId = eventManger::startBuildingEvent()->
                setEventRoute("App update/move camera " + std::to_string(parentId))->
                setEventCallback(OnUpdate)->
                setEntityID(parentId)->
                setWindowId(winId)->add();
                
        OnMouseScrolledId = eventManger::startBuildingEvent()->
                setEventRoute("Window resize/handel view resize " + std::to_string(parentId))->
                setEventCallback(OnWindowResized)->
                setEntityID(parentId)->
                setWindowId(winId)->add();
                
        OnWindowResizedId = eventManger::startBuildingEvent()->
                setEventRoute("Mouse scrolled/change zoom level " + std::to_string(parentId))->
                setEventCallback(OnMouseScrolled)->
                setEntityID(parentId)->
                setWindowId(winId)->add();
         
    }
    
    void orthographicCameraControler::end()
    {     
    }


    coreCamera *orthographicCameraControler::getCamera()
    {
        return &camera;
    }
}