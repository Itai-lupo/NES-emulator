#include "entity.h"
#include <stdlib.h>

namespace raftelGraphicEngine
{
    std::vector<IEntity*> entityManger::entitys = std::vector<IEntity*>();


    void entityManger::init()
    {
        entityManger::entitys = std::vector<IEntity*>();
    }

    void entityManger::close()
    {
        entityManger::entitys.clear();
    }

    int entityManger::addEntity(IEntity *entityToAdd)
    {
        entityToAdd->id = entityManger::entitys.size();
        entityManger::entitys.push_back(entityToAdd);
        return entityManger::entitys.size() - 1;
    }

    IEntity *entityManger::getEntityById(raftelId id)
    {
        return  entityManger::entitys[id];
    }

}