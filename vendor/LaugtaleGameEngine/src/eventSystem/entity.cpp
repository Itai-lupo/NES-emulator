#include "entity.h"
#include <stdlib.h>
#include "LTEError.h"
#include "logger.h"
namespace LTE
{
    gameObject::gameObjectBuilder *entityManger::builder = nullptr;
    std::vector<gameObject*> entityManger::entitys = std::vector<gameObject*>();
    u_int32_t entityManger::nextGameObjectId = 1;


    void entityManger::init()
    {
        entityManger::entitys = std::vector<gameObject*>();
        builder = new gameObject::gameObjectBuilder();
    }

    void entityManger::close()
    {
        entityManger::entitys.clear();
        entityManger::nextGameObjectId = 1;
    }

    entityTaleId entityManger::addEntity(std::function<void(gameObject::gameObjectBuilder *Builder)> buildGameObject)
    {
        builder->reset();
        buildGameObject(builder);
        gameObject *res = builder->build(entityManger::nextGameObjectId);
        entityManger::entitys.push_back(res);
        entityManger::nextGameObjectId++;
        return res->getId();
    }

    gameObject *entityManger::getEntityByName(const std::string& name)
    {
        gameObject *e = nullptr;
        for (uint64_t i = 0; i < entityManger::entitys.size(); i++)
        {
            if(entityManger::entitys[i]->getName()  == name)
                return entityManger::entitys[i];
        }
        
        throw GameObjectNotFoundException("game object with name:" + name + "wasn't found");
        return  NULL;
        
    }

    gameObject *entityManger::getEntityById(entityTaleId id)
    {
        gameObject *e = nullptr;
        for (uint64_t i = 0; i < entityManger::entitys.size(); i++)
        {
            if(entityManger::entitys[i]->getId()  == id)
                return entityManger::entitys[i];
        }
        
        throw new GameObjectNotFoundException("game object with id: " + std::to_string(id) + " wasn't found");
        return  NULL;
    }

    void entityManger::removeEntityById(entityTaleId id)
    {
        for (uint64_t i = 0; i < entitys.size(); i++)
        {
            if(entitys[i]->getId()  == id)
            {
                gameObject *e = entitys[i]; 
                entitys.erase(entitys.begin() + i);
                e->end();
                delete e;
            }
        }
        
    }
}