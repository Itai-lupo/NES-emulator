#pragma once
#include <functional>
#include <vector>
#include "core.h"

namespace LaughTaleEngine
{
    struct IEntity
    {
        public:
            IEntity(){}
            int x = 0, y = 0, z = 0, width = 0, hight = 0;
            
            entityTaleId id;
    };

    class entityManger
    {
        private:
            static std::vector<IEntity*> entitys;
        public:
            static void init();
            static void close();
            static entityTaleId addEntity(IEntity *eventToAdd);
            static IEntity *getEntityById(entityTaleId id);
            static void removeEntityById(entityTaleId id);

    };
}