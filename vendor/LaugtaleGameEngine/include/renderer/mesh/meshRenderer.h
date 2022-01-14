#pragma once
#include "core.h"
#include "windowManger.h"
#include "component.h"
#include "mesh.h"
#include "material.h"
#include "renderApi.h"
#include "entity.h"
#include "scene.h"
#include "shader.h"
#include <algorithm>
#include <vector>

namespace LTE
{
    class meshRenderer: public component
    {
        private:
            shader *s;
            std::string shaderPath;
            glm::mat4 trans;

            void setTransform(glm::mat4 trans)
            {
                s->setUniformMat4f("transform", trans);
                this->trans = trans; 
            }


            void setTransform(transform *trans)
            {
                this->trans =   glm::translate(glm::mat4(1.0f), trans->getPostion()) * 
                                    glm::rotate(glm::mat4(1.0f), trans->getRotation().x, { 1.0f, 0.0f, 0.0f}) *
                                    glm::rotate(glm::mat4(1.0f), trans->getRotation().y, { 0.0f, 1.0f, 0.0f}) *
                                    glm::rotate(glm::mat4(1.0f), trans->getRotation().z, { 0.0f, 0.0f, 1.0f}) *
                                    glm::scale(glm::mat4(1.0f), trans->getScale());
                                    
                s->setUniformMat4f("transform", this->trans);
            }
            

        public:
            meshRenderer(const std::string& shaderPath): shaderPath(shaderPath)
            {

            }


            virtual void init(gameObject *parent) override
            {
                window *win = windowManger::getWindow(winId);
                win->activeScene->objects->push_back(parent);
                s = win->assetLibrary->getAsset<shader>(shaderPath.c_str());
            }

            void setShader(const std::string& shaderPath)
            {
                this->shaderPath = shaderPath;
                s = windowManger::getWindow(winId)->assetLibrary->getAsset<shader>(shaderPath.c_str());
            }

            virtual void end() override 
            {
                windowManger::getWindow(winId)->activeScene->objects->erase(
                    std::remove_if(
                        windowManger::getWindow(winId)->activeScene->objects->begin(),
                        windowManger::getWindow(winId)->activeScene->objects->end(),
                        [=](gameObject *g) -> bool { return g->getId() == parentId; }
                    )
                );
            }

            // void render(renderApi *api, const glm::mat4& MVP)
            // {
            //     try
            //     {
            //         gameObject *objectToRender = entityManger::getEntityById(parentId);
            //         if(objectToRender == nullptr) return;
            //         mesh *ObjectMesh = objectToRender->getComponent<mesh>();
            //         material *ObjectMaterial = objectToRender->getComponent<material>();

            //         s->bind();
            //         ObjectMesh->bind();
            //         ObjectMaterial->bind(s, {0});
            //         s->setUniformMat4f("viewProjection", MVP);
            //         setTransform(entityManger::getEntityById(parentId)->getTransform());

            //         api->DrawIndexed(ObjectMesh->getCount());
            //     }
            //     catch(const std::exception& e)
            //     {
            //         LAUGHTALE_ENGINR_LOG_WARNING("coul'd not render game object because: " << e.what());
            //     }
            // }
    };
}