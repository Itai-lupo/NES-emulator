#include "materialsManger.h"
#include <algorithm>
#include "logger.h"
#include "material.h"

namespace LaughTaleEngine
{
    std::vector<material *> *materialsManger::materials;
    matrialId materialsManger::nextId = 0;

    void materialsManger::init()
    {
        materials = new std::vector<material *>();
    }

    void materialsManger::close()
    {
        materials->clear();
    }


    matrialId materialsManger::addMatrial(material *toAdd)
    {
        materials->push_back(toAdd);
        toAdd->id = nextId;
        nextId++;
        return toAdd->id; 
    }

    void materialsManger::bind(matrialId id, shader *shaderToBindTo)
    {
        std::vector<material *>::iterator m;
        m =  std::find_if(
            materials->begin(),
            materials->end(),
            [=] (material *m) -> bool { return m->id == id; }
        );
        if(m != materials->end())
            return (*m)->bind(shaderToBindTo);
        
        LAUGHTALE_ENGINR_LOG_WARNING("faild to bind matrial: matrial wasn't found");
    }

    
}