#pragma once
#include <functional>
#include <vector>

namespace raftelGraphicEngine
{
    struct IEntity
    {
        public:
            IEntity(){}
            int x = 0, y = 0, z = 0;
            int spriteId = 0;
            int vertexShaderId = 0;
            int fragmentShaderId = 0;
            int id;
    };

    class entityManger
    {
        private:
            static std::vector<IEntity*> entitys;
        public:
            static void init();
            static void close();
            static int addEntity(IEntity *eventToAdd);
            static IEntity *getEntityById(int id);
    };
}