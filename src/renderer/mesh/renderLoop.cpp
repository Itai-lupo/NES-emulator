#include "renderLoop.h"
#include "renderer.h"
#include "window.h"

#include <algorithm>

namespace LaughTaleEngine
{
    std::vector<mesh *> *renderLoop::meshs;

    void renderLoop::init()
    {
        meshs = new std::vector<mesh *>();
    }
    
    void renderLoop::close()
    {
        for ( mesh *m: *meshs)
        {
            entityManger::removeEntityById(m->id);
            if(m->isActive)
               eventManger::removeEvent(events::AppRender, m->onRenderId);
        }
        
    }       

    void renderLoop::renderMesh(LaughTaleEngine::IEntity *eventEntity, LaughTaleEngine::IEventData *sendor)
    {
        LaughTaleEngine::mesh *meshToRender = static_cast<LaughTaleEngine::mesh *>(eventEntity);
        LaughTaleEngine::renderer *r = LaughTaleEngine::windowManger::getRenderer(meshToRender->getWindowId());
            
        meshToRender->bind();            
        r->Submit(meshToRender);
    }

    entityTaleId renderLoop::addMesh(mesh *meshToAdd)
    {
        meshs->push_back(meshToAdd);
        meshToAdd->id = entityManger::addEntity(meshToAdd);
        return meshToAdd->id;
    }
    
    void renderLoop::active(entityTaleId id)
    {
        mesh *meshToActive = getMesh(id);

        if(meshToActive != NULL && !meshToActive->isActive)
        {
            meshToActive->onRenderId = eventManger::addEvent( events::AppRender, renderMesh, meshToActive->id, meshToActive->getWindowId());
            meshToActive->isActive = true;
        }

    }
    
    void renderLoop::deActive(entityTaleId id)
    {
        mesh *meshToDeActive = getMesh(id);
        
        if(meshToDeActive != NULL && !meshToDeActive->isActive)
        {
            eventManger::removeEvent(events::AppRender, meshToDeActive->onRenderId);
            meshToDeActive->isActive = false;    
        }

    }
    
    void renderLoop::remove(entityTaleId id)
    {
        mesh *meshToRemove = getMesh(id);
        entityManger::removeEntityById(meshToRemove->id);
        if(meshToRemove->isActive)
            eventManger::removeEvent(events::AppRender, meshToRemove->onRenderId);
        
        std::remove_if(
            meshs->begin(),
            meshs->end(),
            [=](mesh *m) -> bool { return m->id == id; }
        );

    }

    mesh *renderLoop::getMesh(entityTaleId id)
    {
        return *std::find_if(
            meshs->begin(),
            meshs->end(),
            [=](mesh *m) -> bool { return m->id == id; }
        );
    }

}

