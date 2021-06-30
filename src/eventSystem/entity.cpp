#include "entity.h"
#include <stdlib.h>

#include "logger.h"
namespace LTE
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
        IEntity *e = nullptr;
        int j = -1;
        for (uint64_t i = 0; i < entityManger::entitys.size(); i++)
        {
            if(entityManger::entitys[i]->id  == id){
                LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("2 with the same id: (" << j << ", " << i << ") with id: " << id  , j != -1);
                j = i;
                e = entityManger::entitys[i];
            }
        }
        
        return  e;
    }

    void entityManger::removeEntityById(entityTaleId id)
    {
        for (uint64_t i = 0; i < entityManger::entitys.size(); i++)
        {
            if(entityManger::entitys[i]->id  == id)
            {
                entityManger::entitys.erase(entityManger::entitys.begin() + i);
            }
        }
        
    }

}