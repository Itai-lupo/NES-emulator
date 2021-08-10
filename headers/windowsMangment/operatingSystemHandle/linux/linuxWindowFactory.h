#pragma once

#include <string>

#include "core.h"
#include "coreWindowFactory.h"

#include "linuxInterface.h"
#include "openGLrenderApi.h"
#include "scene.h"
#include "material.h"
#include "orthographicCameraControler.h"
#include "entity.h"

namespace LTE
{
    class linuxWindowFactory: public coreWindowFactory
    {
        protected:
            virtual void build()
            {
                prodect->operatingSystemPipLine = new linuxInterface(prodect);
                prodect->renderPipLine = new openGLRenderApi();
                prodect->renderPipLine->init();
                prodect->activeScene = new scene();
                prodect->activeScene->backgroundColor = new material(glm::vec4({0.1f, 0.1f, 0.5f, 1.0f}));
                prodect->sceneCollider = new colliderSystem2D();
                entityTaleId camId = entityManger::addEntity([=, this](gameObject::gameObjectBuilder *builder){
                    builder->setObjectName("default camera")->
                        setWindowId(prodect->id)->
                        setObjectTransform(glm::mat4(0.0f))->
                        addComponent(new orthographicCameraControler((float)prodect->Width/(float)prodect->Height));
                });
                prodect->activeScene->camera = entityManger::getEntityById(camId);
                prodect->activeScene->objects = new std::vector<gameObject*>();
            }
    };
}