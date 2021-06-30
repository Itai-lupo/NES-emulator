#pragma once
#include "core.h"
#include "coreCameraControler.h"
#include "orthographicCamera.h"
#include "entity.h"
#include "events.h"

namespace LTE
{
    class orthographicCameraControler: public coreCameraControler
    {
        private:

            float aspectRatio;
            float zoomLevel = 1.0f;
            bool useCameraRotation;

            glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
            float cameraRotation = 0.0f;
            float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;

            orthographicCamera camera;

            eventLaughId OnMouseScrolledId; 
            eventLaughId OnWindowResizedId; 
            eventLaughId OnUpdateId; 
            entityTaleId CameraEntityId;

            static void OnMouseScrolled(IEntity *cameraInfo, coreEventData *sendor);
            static void OnWindowResized(IEntity *cameraInfo, coreEventData *sendor);
            static void OnUpdate(IEntity *cameraInfo, coreEventData *sendor);


        public:
            orthographicCameraControler(float aspectRatio, windowPieceId window, bool useCameraRotation = false);
            ~orthographicCameraControler();
    
            virtual coreCamera *getCamera() override;
    };
}