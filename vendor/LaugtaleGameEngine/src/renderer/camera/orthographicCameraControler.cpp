#include "orthographicCameraControler.h"
#include "logger.h"
#include "entity.h"
#include "MouseButtonCodes.h"

#include "window.h"

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
        return;
        onUpdateData *eventData = dynamic_cast<onUpdateData *>(sendor);   
        orthographicCameraControler *thisCamera = cameraInfo->getComponent<orthographicCameraControler>();


        if(eventData->win->inputManger->isMouseButtonPressed(LT_MOUSE_BUTTON_MIDDLE))
        {
            glm::vec3 cameraPostion = thisCamera->camera.getPosition();
            cameraPostion.x += ((eventData->win->inputManger->GetMouseX() - thisCamera->mouseLastPostion.x) * (1 /  (float)eventData->win->Width)) * eventData->DeltaTime;
            cameraPostion.y += ((eventData->win->inputManger->GetMouseY() - thisCamera->mouseLastPostion.y) * (1 / (float)eventData->win->Height)) * eventData->DeltaTime;

            thisCamera->camera.SetPosition(cameraPostion);
            LAUGHTALE_ENGINR_LOG_INFO(eventData->win->inputManger->GetMouseX()  << ", " << cameraPostion.y);
        }
        thisCamera->mouseLastPostion = eventData->win->inputManger->GetMousePosition();
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