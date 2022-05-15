#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "scene.h"

namespace LTE
{
    void scene::removeById(entityTaleId id)
    {        
        
        // objects->erase(std::remove_if(
        //     objects->begin(), 
        //     objects->end(),
        //     [=](gameObject* go)-> bool { 
        //             if(go->getId() == id) 
        //             {
        //                 entityManger::removeEntityById(go->getId());
        //                 prefabLibary::getPrefab(go->getName())->onSceneExit(go);
        //                 delete go;
        //                 return true; 
        //             }
        //             return false;
        //             }), objects->end());
    }

}