#pragma once
#include <functional>
#include <vector>
#include "core.h"

namespace LaughTaleEngine
{
    struct IEntity
    {
        public:
            virtual ~IEntity() = default;
            
            entityTaleId id;
    };

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
}