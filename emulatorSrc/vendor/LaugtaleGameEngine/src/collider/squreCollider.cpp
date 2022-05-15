#include "squreCollider.h"
#include "colliderSystem2D.h"
#include "windowManger.h"
#include "entity.h"
#include "events.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

namespace LTE
{
    squreCollider::squreCollider()
    {
    }

    squreCollider::~squreCollider()
    {

    }

    transform *squreCollider::getBonds() const
    {
        return entityManger::getEntityById(parentId)->getTransform();
    }


    void squreCollider::init(gameObject *parent)
    {
        eventManger::addCoustemEventsRoute("collider/" + parent->getName() + "/");
        windowManger::getWindow(winId)->sceneCollider->addSqureCollider(parent);
    }

    void squreCollider::end()
    {
        windowManger::getWindow(winId)->sceneCollider->removeSqureCollider(parentId);
    }  


    bool checkXBonds(float selfRight, float selfLeft, float otherRight, float otherLeft)
    {
        return (
            (selfRight <= otherRight && selfRight >= otherLeft) ||
            (selfLeft <= otherRight && selfLeft >= otherLeft) ||
            (otherRight <= selfRight && otherRight >= selfLeft) ||
            (otherLeft <= selfRight && otherLeft >= selfLeft)
            );
    }  

    bool checkYBonds(float selfTop, float selfBottom, float otherTop, float otherBottom)
    {
        return (
            (selfTop <= otherTop && selfTop >= otherBottom) ||
            (selfBottom <= otherTop && selfBottom >= otherBottom) ||
            (otherTop <= selfTop && otherTop >= selfBottom) ||
            (otherBottom <= selfTop && otherBottom >= selfBottom)
            );
    }

    bool squreCollider::operator == (squreCollider& s)
    {
        try
        {
            transform *selfBonds = getBonds();
            transform *otherBonds = s.getBonds();

            glm::vec3 selfDownRightBonds = selfBonds->getPostion() - selfBonds->getScale()/2.0f;
            glm::vec3 selfUpLeftBonds = selfBonds->getPostion() + selfBonds->getScale()/2.0f;

            glm::vec3 otherDownRightBonds = otherBonds->getPostion() - otherBonds->getScale()/2.0f;
            glm::vec3 otherUpLeftBonds = otherBonds->getPostion() + otherBonds->getScale()/2.0f;

            bool isWithInBonds =  
                checkYBonds(selfUpLeftBonds.y, selfDownRightBonds.y, otherUpLeftBonds.y, otherDownRightBonds.y) &&
                checkXBonds(selfUpLeftBonds.x, selfDownRightBonds.x, otherUpLeftBonds.x, otherDownRightBonds.x);
            
            bool AlradyInCollisionWith = std::find_if(
                InCollisionWith.begin(), 
                InCollisionWith.end(),
                [=, this](squreCollider *a) -> bool { return a->parentId == s.parentId; }) != InCollisionWith.end();
                
            if (isWithInBonds && !AlradyInCollisionWith)
            {
                InCollisionWith.push_back(&s);
                return true;
            }

            if(!isWithInBonds && AlradyInCollisionWith)
                InCollisionWith.erase(std::remove_if(
                    InCollisionWith.begin(), 
                    InCollisionWith.end(),
                    [=, this](squreCollider *a) -> bool { return a->parentId == s.parentId; }));
        }
        catch(const std::exception& e)
        {
            LAUGHTALE_ENGINR_LOG_WARNING("coul'd not calculate collsion betwin game object because: " << e.what());
        }
        
        
        return false;
        
    }

} 