#include "colliderSystem2D.h"
#include "events.h"
#include "colliderEventData.h"
#include <algorithm>

namespace LTE
{
    
    void colliderSystem2D::init()
    {

    }
    
    void colliderSystem2D::close()
    {

    }

    void colliderSystem2D::removeSqureCollider(entityTaleId id)
    {
        squreColliders.erase(std::remove_if(
            squreColliders.begin(),
            squreColliders.end(),
            [=](gameObject *g) -> bool { return g->getId() == id; }
        ));
    }

    void colliderSystem2D::checkCollision()
    {
        for (uint32_t i = 0; i < squreColliders.size(); i++)
        {
            for (uint32_t j = i + 1; j < squreColliders.size(); j++)
            {
                if(*squreColliders[i]->getComponent<squreCollider>() == *squreColliders[j]->getComponent<squreCollider>())
                {
                    colliderEventData *IToJ = new colliderEventData(squreColliders[i]->getName(), squreColliders[j]);
                    colliderEventData *JToI = new colliderEventData(squreColliders[j]->getName(), squreColliders[i]);
                    eventManger::trigerEvent(IToJ);
                    eventManger::trigerEvent(JToI);
                    delete IToJ;
                    delete JToI;
                }
            }
            
        }
        
    }

} 
