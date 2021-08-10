#pragma once
#include "coreEventData.h"
#include "entity.h"

namespace LTE
{
    struct colliderEventData: public coreEventData
    {
        public:
            colliderEventData(const std::string& objectName, gameObject *target): target(target), coreEventData("collider/" + objectName + "/") {}

            gameObject *target;
    };
    
} 
