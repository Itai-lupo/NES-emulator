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

            glm::vec2 mouseLastPostion = {0, 0};

            glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };
            float cameraRotation = 0.0f;
            float cameraTranslationSpeed = 5.0f, cameraRotationSpeed = 180.0f;

            orthographicCamera camera;

            eventLaughId OnMouseScrolledId; 
            eventLaughId OnWindowResizedId; 
            eventLaughId OnUpdateId; 
            entityTaleId CameraEntityId;

            static void OnMouseScrolled(gameObject *cameraInfo, coreEventData *sendor);
            static void OnWindowResized(gameObject *cameraInfo, coreEventData *sendor);
            static void OnUpdate(gameObject *cameraInfo, coreEventData *sendor);


        public:
            orthographicCameraControler(float aspectRatio, bool useCameraRotation = false);
            ~orthographicCameraControler();
    
            virtual coreCamera *getCamera() override;

            virtual void init(gameObject *parent) override;
            virtual void end() override;

    };
}