#pragma once

#include <vector>
#include <string>

#include "core.h"
#include "coreCameraControler.h"
#include "material.h"

#include "colliderSystem2D.h"


namespace LTE
{
    class scene
    {
        public:
            scene(){}
            ~scene(){}

            gameObject *camera;
            std::vector<gameObject*> *objects;
            material *backgroundColor;

            void removeById(entityTaleId id);
            
            std::vector<gameObject*> getObjectsByName(const std::string& objectName)
            {
                std::vector<gameObject*> objectWithName;
                for(gameObject *obj: *objects)
                {
                    if(obj->getName() == objectName)
                        objectWithName.push_back((obj));
                }
                return objectWithName;
            }

            template<typename objectType> std::vector<objectType> getObjectsByName(const std::string& objectName)
            {
                std::vector<objectType> objectWithName;
                for(gameObject *obj: *objects)
                {
                    if(obj->getName() == objectName)
                        objectWithName.push_back(static_cast<objectType>(obj));
                }
                return objectWithName;
            }
    };

}