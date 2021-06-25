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
        meshs->clear();
        
    }       

    void renderLoop::renderMesh(LaughTaleEngine::IEntity *eventEntity, LaughTaleEngine::IEventData *sendor)
    {
        LaughTaleEngine::mesh *meshToRender = static_cast<LaughTaleEngine::mesh *>(eventEntity);
        LaughTaleEngine::renderer *r = LaughTaleEngine::windowManger::getRenderer(meshToRender->getWindowId());
        
        r->Submit(meshToRender);
    }

    entityTaleId renderLoop::addMesh(mesh *meshToAdd)
    {
        entityManger::addEntity(meshToAdd);
        meshs->push_back(meshToAdd);
        return meshToAdd->getId();
    }
    
    void renderLoop::active(entityTaleId id)
    {
        mesh *meshToActive = getMesh(id);

        if(meshToActive != nullptr && !meshToActive->isActive)
        {
            meshToActive->onRenderId = eventManger::addEvent( events::AppRender, renderMesh, meshToActive->getId(), meshToActive->getWindowId());
            meshToActive->isActive = true;
        }

    }
    
    void renderLoop::deActive(entityTaleId id)
    {
        mesh *meshToDeActive = getMesh(id);
        
        if(meshToDeActive != nullptr && !meshToDeActive->isActive)
        {
            eventManger::removeEvent(events::AppRender, meshToDeActive->onRenderId);
            meshToDeActive->isActive = false;    
        }

    }
    
    void renderLoop::remove(entityTaleId id)
    {
        mesh *meshToRemove = getMesh(id);

        if(meshToRemove->isActive)
            eventManger::removeEvent(events::AppRender, meshToRemove->onRenderId);
        
        entityManger::removeEntityById(meshToRemove->getId());
        meshs->erase(std::remove_if(
            meshs->begin(),
            meshs->end(),
            [=](mesh *m) -> bool { return m->getId() == id; }
        ), meshs->end());
    }

    mesh *renderLoop::getMesh(entityTaleId id)
    {
        return *std::find_if(
            meshs->begin(),
            meshs->end(),
            [=](mesh *m) -> bool { return m->getId() == id; }
        );
    }

}

