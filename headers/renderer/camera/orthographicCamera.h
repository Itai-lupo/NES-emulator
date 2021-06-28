#pragma once
#include "core.h"
#include "coreCamera.h"
#include <glm/glm.hpp>

namespace LaughTaleEngine
{
    class orthographicCamera: public coreCamera
    {
        private:
            glm::mat4 projectionMatrix;
            glm::mat4 viewMatrix;
            glm::mat4 viewProjectionMatrix;

            glm::vec3 position = { 0.0f, 0.0f, 0.0f };
            float rotation = 0.0f;

            void recalculateViewMatrix();

        public:
            orthographicCamera(float left, float right, float bottom, float top);
            
            virtual void SetProjection(float left, float right, float bottom, float top) override;

            virtual const glm::vec3& getPosition()  override { return position; }
            virtual void SetPosition(const glm::vec3& position)  override;
 
            virtual float getRotation()  override { return rotation; }
            virtual void setRotation(float rotation)  override;
 
            virtual const glm::mat4& getProjectionMatrix()  override { return projectionMatrix; }
            virtual const glm::mat4& getViewMatrix()  override { return viewMatrix; }
            virtual const glm::mat4& getViewProjectionMatrix() override { return viewProjectionMatrix; }
    };
}