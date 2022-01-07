#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <functional>

#include "core.h"
#include "logger.h"
#include "component.h"
#include "transform.h"
#include "LTEError.h"

namespace LTE
{
    class gameObject
    {     
        public:
            class gameObjectBuilder
            {
                protected:
                    gameObject *prodect;
                
                public:
                    gameObjectBuilder()
                    {
                        prodect = new gameObject();
                    }

                    gameObjectBuilder *setObjectName(const std::string& name)
                    {
                        prodect->name = name;
                        return this;
                    }

                    gameObjectBuilder *setWindowId(windowPieceId winId)
                    {
                        prodect->winId = winId;
                        return this;
                    }

                    gameObjectBuilder *setObjectTransform(glm::mat3 trans)
                    {
                        prodect->ObjectTransform = new transform(trans);
                        return this;
                    }

                    gameObjectBuilder *addComponent(component* toAdd)
                    {
                        prodect->components.push_back(toAdd);
                        return this;
                    }     

                    gameObjectBuilder *reset()
                    {
                        prodect = new gameObject();
                        return this;
                    }


                    gameObject *build(entityTaleId id)
                    {
                        for(component *c: prodect->components){
                            c->setParent(id, prodect->winId);
                            c->init(prodect);
                        }
                        prodect->id = id;
                        return prodect;
                    }
 
            };

        private:
            entityTaleId id;
            std::string name = "";
            transform *ObjectTransform;
            windowPieceId winId = 0;

            std::vector<component*> components;
            gameObject(){}

        public:    
            const std::string& getName(){ return name; }
            const entityTaleId getId(){ return id; }
            
            template<typename T> 
            T *getComponent()
            {
                if(id == 0 || !ObjectTransform)
                    throw new ComponentNotFoundException("entitys with id 0 have no data ");

                for (auto comp: components)
                {
                    if(comp && dynamic_cast<T*>(comp))
                        return dynamic_cast<T*>(comp); 
                }
                throw new ComponentNotFoundException("can't find Component of type: " + std::string(typeid(T).name()));
                return NULL;
            }    

            template<typename T> 
            void setComponent(component *componentToAdd)
            {
                if(id == 0 || !ObjectTransform)
                    throw new ComponentNotFoundException("entitys with id 0 have no data ");

                for (auto comp: components)
                {
                    if(comp && dynamic_cast<T*>(comp))
                    {
                        comp = componentToAdd; 
                        return;    
                    }
                }

                components.push_back(componentToAdd);
            }    

            template<typename T> 
            void removeComponent()
            {
                if(id == 0 || !ObjectTransform)
                    throw new ComponentNotFoundException("entitys with id 0 have no data ");
                std::vector<component*>::iterator it = std::remove_if(
                        components.begin(), 
                        components.end(),
                        [](component *comp) -> bool
                        {
                            return comp && dynamic_cast<T*>(comp);
                        });

                if(it != components.end())
                    components.erase(it);
            }    

            void end()
            {
                for(auto c: components)
                {
                    c->end();
                    delete c;
                }
                components.clear();
                ObjectTransform->end();
                delete ObjectTransform;
            }

            transform *getTransform()
            {
                return ObjectTransform;
            }

            virtual ~gameObject()
            {
                id = 0;
            }
            
    };

    class entityManger
    {
        private:
            static gameObject::gameObjectBuilder *builder;
            static std::vector<gameObject*> entitys;
            static uint32_t nextGameObjectId;
        public:
            static void init();
            static void close();
            static entityTaleId addEntity(std::function<void(gameObject::gameObjectBuilder *Builder)> buildGameObject);
            static gameObject *getEntityByName(const std::string& name);
            static gameObject *getEntityById(entityTaleId id);
            static void removeEntityById(entityTaleId id);

    };


}