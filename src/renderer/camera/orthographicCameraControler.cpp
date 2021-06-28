#include "orthographicCameraControler.h"
#include "logger.h"
#include "entity.h"

namespace LaughTaleEngine
{
    struct cameraEntity: IEntity
    {
        cameraEntity(){}
        orthographicCamera *camera;
        float *aspectRatio;
        float *zoomLevel;
        bool *useCameraRotation;

        glm::vec3 *cameraPosition;
        float *cameraRotation;
        float *cameraTranslationSpeed, *cameraRotationSpeed;

    };

    void orthographicCameraControler::OnMouseScrolled(IEntity *cameraInfo, IEventData *sendor)
    {
        cameraEntity *camera = dynamic_cast<cameraEntity *>(cameraInfo);  
        mouseScrollData *eventData = dynamic_cast<mouseScrollData *>(sendor);

        (*camera->zoomLevel) -= eventData->yOffset * 0.25f;
		(*camera->zoomLevel) = std::max((*camera->zoomLevel), 0.25f);
        LAUGHTALE_ENGINR_LOG_INFO((*camera->zoomLevel) << ", " << (*camera->aspectRatio) << ", " << *camera->cameraRotationSpeed);
		camera->camera->SetProjection(-(*camera->aspectRatio) * (*camera->zoomLevel), (*camera->aspectRatio) * (*camera->zoomLevel), -(*camera->zoomLevel), (*camera->zoomLevel));
    }

    void orthographicCameraControler::OnWindowResized(IEntity *cameraInfo, IEventData *sendor)
    {
        cameraEntity *camera = dynamic_cast<cameraEntity *>(cameraInfo);  
        WindowResizeData *eventData = dynamic_cast<WindowResizeData *>(sendor);

        (*camera->aspectRatio) = (float)eventData->windowWidth / (float)eventData->windowHeight;
		(*camera->camera).SetProjection(-(*camera->aspectRatio) * (*camera->zoomLevel), (*camera->aspectRatio) * (*camera->zoomLevel), -(*camera->zoomLevel), (*camera->zoomLevel));
    }

    void orthographicCameraControler::OnUpdate(IEntity *cameraInfo, IEventData *sendor)
    {
        cameraEntity *camera = dynamic_cast<cameraEntity *>(cameraInfo);  
        onUpdateData *eventData = dynamic_cast<onUpdateData *>(sendor);   
    }

    orthographicCameraControler::orthographicCameraControler(float aspectRatio, windowPieceId window, bool useCameraRotation)
        :aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), useCameraRotation(useCameraRotation)
    {

        cameraEntity *cam = new cameraEntity();
        cam->camera = &camera;
        cam->aspectRatio = &this->aspectRatio;

        cam->zoomLevel = &zoomLevel;
        cam->useCameraRotation = &useCameraRotation;
        cam->cameraPosition = &cameraPosition;
        cam->cameraRotation = &cameraRotation;
        cam->cameraTranslationSpeed = &cameraTranslationSpeed;
        cam->cameraRotationSpeed = &cameraRotationSpeed;

        CameraEntityId = entityManger::addEntity(cam);
        OnUpdateId = eventManger::addEvent(events::AppUpdate, OnUpdate, CameraEntityId);
        OnMouseScrolledId = eventManger::addEvent(events::MouseScrolled, OnMouseScrolled, CameraEntityId, window);
        OnWindowResizedId = eventManger::addEvent(events::WindowResize, OnWindowResized, CameraEntityId, window);
    }

    orthographicCameraControler::~orthographicCameraControler()
    {
        eventManger::removeEvent(events::AppUpdate, OnUpdateId);
        eventManger::removeEvent(events::WindowResize, OnWindowResizedId);
        eventManger::removeEvent(events::MouseScrolled, OnMouseScrolledId);
        delete entityManger::getEntityById(CameraEntityId);
        entityManger::removeEntityById(CameraEntityId);

    }

    coreCamera *orthographicCameraControler::getCamera()
    {
        return &camera;
    }
}