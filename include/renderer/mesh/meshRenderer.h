#pragma once
#include "core.h"
#include "windowManger.h"
#include "component.h"
#include "mesh.h"
#include "material.h"
#include "renderApi.h"
#include "entity.h"
#include "scene.h"

#include <algorithm>

namespace LTE
{
    class meshRenderer: public component
    {
        private:
    
        public:
            virtual void init(gameObject *parent) override
            {
                windowManger::getWindow(winId)->activeScene->objects->push_back(parent);
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

            void render(renderApi *api, const glm::mat4& MVP)
            {
                gameObject *objectToRender = entityManger::getEntityById(parentId);
                mesh *ObjectMesh = objectToRender->getComponent<mesh>();
                material *ObjectMaterial = objectToRender->getComponent<material>();

                ObjectMesh->bind();
                ObjectMaterial->bind(ObjectMesh->getShader(), {0});
                ObjectMesh->getShader()->setUniformMat4f("viewProjection", MVP);

                api->DrawIndexed(ObjectMesh->getCount());
            }
    };
}