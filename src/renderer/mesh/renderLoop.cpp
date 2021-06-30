#include "renderLoop.h"
#include "renderer.h"
#include "window.h"

#include <algorithm>

namespace LTE
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

    void renderLoop::renderMesh(LTE::IEntity *eventEntity, LTE::coreEventData *sendor)
    {
        LTE::mesh *meshToRender = static_cast<LTE::mesh *>(eventEntity);
        LTE::renderer *r = LTE::windowManger::getRenderer(meshToRender->getWindowId());
        
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
            event *onAppRenderEvent = event::eventBuilder::startBuilding()->
                            setEventType(events::AppRender)->
                            setEventCallback(renderMesh)->setEntityID(meshToActive->getId())->
                            setWindowId(meshToActive->getWindowId())->build();
          
            meshToActive->onRenderId = eventManger::addEvent(onAppRenderEvent);
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

