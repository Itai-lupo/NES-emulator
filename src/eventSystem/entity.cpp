#include "entity.h"
#include <stdlib.h>

namespace LaughTaleEngine
{
    std::vector<IEntity*> entityManger::entitys = std::vector<IEntity*>();
    u_int32_t entityManger::nextEventId = 0;


    void entityManger::init()
    {
        entityManger::entitys = std::vector<IEntity*>();
    }

    void entityManger::close()
    {
        entityManger::entitys.clear();
        entityManger::nextEventId = 0;
    }

    entityTaleId entityManger::addEntity(IEntity *entityToAdd)
    {
        entityToAdd->id = entityManger::nextEventId;
        entityManger::nextEventId++;
        entityManger::entitys.push_back(entityToAdd);
        return entityToAdd->id;
    }

    IEntity *entityManger::getEntityById(entityTaleId id)
    {
        for (uint64_t i = 0; i < entityManger::entitys.size(); i++)
        {
            if(entityManger::entitys[i]->id  == id)
                return entityManger::entitys[i];
        }
        
        return  nullptr;
    }

    void entityManger::removeEntityById(entityTaleId id)
    {
        for (uint64_t i = 0; i < entityManger::entitys.size(); i++)
        {
            if(entityManger::entitys[i]->id  == id)
            {
                entityManger::entitys.erase(entityManger::entitys.begin() + i);
                return;
            }
        }
    }

}