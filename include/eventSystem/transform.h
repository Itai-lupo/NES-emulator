#pragma once
#include <functional>
#include <vector>
#include "glm/glm.hpp"

#include "core.h"
#include "events.h"
#include "component.h"

namespace LTE
{
    class gameObject;
    class transform: public component
    {
        private:
            glm::vec3 postion;
            glm::vec3 rotation;
            glm::vec3 scale;
            
            gameObject *parent;
        
        public:
            transform(glm::mat3 trans):
                postion(trans[0]), rotation(trans[1]), scale(trans[2])
            {
                
            }

            virtual void init(gameObject *parent)
            {
                this->parent = parent;
            }

            virtual void end()
            {

            }

            void setPostion(const glm::vec3& postion)
            {
                this->postion = postion;
            }


            void setRotation(const glm::vec3& rotation)
            {
                this->rotation = rotation;
            }


            void setScale(const glm::vec3& scale)
            {
                this->scale = scale;
            }

            void setXPostion(float postion)
            {
                this->postion.x = postion;
            }

            void setXRotation(float rotation)
            {
                this->rotation.x = rotation;
            }

            void setXScale(float scale)
            {
                this->scale.x = scale;
            }

            void setYPostion(float postion)
            {
                this->postion.y = postion;
            }


            void setYRotation(float rotation)
            {
                this->rotation.y = rotation;
            }


            void setYScale(float scale)
            {
                this->scale.y = scale;
            }

            void setZPostion(float postion)
            {
                this->postion.z = postion;
            }


            void setZRotation(float rotation)
            {
                this->rotation.z = rotation;
            }


            void setZScale(float scale)
            {
                this->scale.z = scale;
            }

            void changeXPostion(float postion)
            {
                this->postion.x += postion;
            }

            void changeXRotation(float rotation)
            {
                this->rotation.x += rotation;
            }

            void changeXScale(float scale)
            {
                this->scale.x += scale;
            }

            void changeYPostion(float postion)
            {
                this->postion.y += postion;
            }


            void changeYRotation(float rotation)
            {
                this->rotation.y += rotation;
            }


            void changeYScale(float scale)
            {
                this->scale.y += scale;
            }

            void changeZPostion(float postion)
            {
                this->postion.z += postion;
            }


            void changeZRotation(float rotation)
            {
                this->rotation.z += rotation;
            }


            void changeZScale(float scale)
            {
                this->scale.z += scale;
            }


            glm::vec3 getPostion()
            {
                return this->postion;
            }


            glm::vec3 getRotation()
            {
                return this->rotation;
            }


            glm::vec3 getScale()
            {
                return this->scale;
            }

    };

}