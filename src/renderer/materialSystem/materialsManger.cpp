#include "materialsManger.h"
#include <algorithm>
#include "logger.h"
#include "material.h"

namespace LaughTaleEngine
{
    std::vector<material *> *materialsManger::materials;
    materialId materialsManger::nextId = 0;

    void materialsManger::init()
    {
        materials = new std::vector<material *>();
    }

    void materialsManger::close()
    {
        materials->clear();
    }


    materialId materialsManger::addMatrial(material *toAdd)
    {
        materials->push_back(toAdd);
        toAdd->id = nextId;
        nextId++;
        return toAdd->id; 
    }

    void materialsManger::bind(materialId id, shader *shaderToBindTo, std::vector<uint32_t> textureSlots)
    {
        std::vector<material *>::iterator m =  std::find_if(
            materials->begin(),
            materials->end(),
            [=] (material *m) -> bool { return m->id == id; }
        );
        if(m != materials->end())
            return (*m)->bind(shaderToBindTo, textureSlots);
        
        LAUGHTALE_ENGINR_LOG_WARNING("faild to bind matrial: matrial wasn't found");
    }

    material *materialsManger::getMaterial(materialId id)
    {
        std::vector<material *>::iterator m =  std::find_if(
            materials->begin(),
            materials->end(),
            [=] (material *m) -> bool { return m->id == id; }
        );

        if(m != materials->end())
            return (*m);
        
        LAUGHTALE_ENGINR_LOG_WARNING("faild to get matrial: matrial wasn't found");
        return nullptr;
    }

    
}