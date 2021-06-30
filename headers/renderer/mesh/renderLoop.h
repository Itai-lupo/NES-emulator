#pragma once

#include <vector>
#include "mesh.h"
#include "core.h"
#include "entity.h"
#include "events.h"

namespace LTE
{
    class renderLoop
    {
    private:
        static std::vector<mesh *> *meshs;
        static void renderMesh(LTE::IEntity *eventEntity, LTE::coreEventData *sendor);
    public:
        static void init();
        static void close();

        static entityTaleId addMesh(mesh *meshToAdd);
        static void active(entityTaleId id);
        static void deActive(entityTaleId id);
        static void remove(entityTaleId id);
        static mesh *getMesh(entityTaleId id);
    };
}
