#pragma once
#include <functional>
#include <vector>
#include "core.h"

namespace LTE
{
    struct IEntity;

    class entityManger
    {
        private:
            static std::vector<IEntity*> entitys;
            static uint32_t nextEventId;
        public:
            static void init();
            static void close();
            static entityTaleId addEntity(IEntity *eventToAdd);
            static IEntity *getEntityById(entityTaleId id);
            static void removeEntityById(entityTaleId id);

    };

    struct IEntity
    {
        private:
            entityTaleId id;
            friend entityManger;

        public:
            entityTaleId getId(){ return id; }
            virtual ~IEntity() = default;
            
    };

}