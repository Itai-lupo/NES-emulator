#pragma once
#include "core.h"
#include <glm/glm.hpp>

namespace LTE
{
    class coreCamera
    {
        public:
            virtual void SetProjection(float left, float right, float bottom, float top) = 0;


            virtual const glm::vec3& getPosition() = 0;
            virtual void SetPosition(const glm::vec3& position) = 0;

            virtual float getRotation() = 0;
            virtual void setRotation(float rotation) = 0;
            
            virtual const glm::mat4& getProjectionMatrix() = 0;
            virtual const glm::mat4& getViewMatrix() = 0;
            virtual const glm::mat4& getViewProjectionMatrix() = 0;
    };
}