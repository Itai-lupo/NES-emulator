#include "shaderManger.h"
#include "logger.h"
#include <algorithm>

namespace LaughTaleEngine
{
    void shaderManger::init()
    {
        shaders = new std::vector<shader *>();
    }

    void shaderManger::close()
    {
        std::for_each(
            shaders->begin(),
            shaders->end(),
            [](shader *i){ free(i); }
        );
        shaders->clear();
    }


    shaderId shaderManger::add(shader *shaderToAdd)
    {
        shaderToAdd->rendererID = shaderToAdd->init();
        shaders->push_back(shaderToAdd);
        return shaderToAdd->rendererID;
    }

    void shaderManger::remove(shaderId shaderToRemove)
    {
        std::remove_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return i->rendererID == shaderToRemove; }
        );
    }

    shader *shaderManger::getShader(shaderId id)
    {
        return (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return i->rendererID == id; }
        ));
    }

    void shaderManger::bind(shaderId id)
    {
       shader *s = getShader(id);
        
        if(s != NULL) s->bind();
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to bind shader: no shader with id: " << id, s == NULL);

    }

    void shaderManger::unbind(shaderId id)
    {
        shader *s = getShader(id);
        
        if(s != NULL) s->unbind();
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to unbind shader: no shader with id: " << id, s == NULL);
    }

    
    void shaderManger::setUniform1i(shaderId id, const std::string& name, int value)
    {
        shader *s = getShader(id);
        
        if(s != NULL) s->setUniform1i(name, value);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to setUniform1i shader: no shader with id: " << id, s == NULL);
    }
    
    void shaderManger::setUniform1f(shaderId id, const std::string& name, float value)
    {
        shader *s = getShader(id);
        
        if(s != NULL) s->setUniform1f(name, value);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to setUniform1f shader: no shader with id: " << id, s == NULL);
    }
    
    void shaderManger::setUniform4f(shaderId id, const std::string& name, float v0, float v1, float v2, float v3)
    {
        shader *s = getShader(id);
        
        if(s != NULL) s->setUniform4f(name, v0, v1, v2, v3);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to setUniform4f shader: no shader with id: " << id, s == NULL);
    }
    
    void shaderManger::setUniformMat4f(shaderId id, const std::string& name, const glm::mat4 &value)
    {
        shader *s = getShader(id);
        
        if(s != NULL) s->setUniformMat4f(name, value);
        LAUGHTALE_ENGINR_CONDTION_LOG_ERROR("faild to setUniformMat4f shader: no shader with id: " << id, s == NULL);
    }

}
