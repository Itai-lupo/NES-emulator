#include "shaderManger.h"
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


    void shaderManger::bind(shaderId id)
    {
       (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return i->rendererID == id; }
        ))->bind();
    }

    void shaderManger::unbind(shaderId id)
    {
        (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return i->rendererID == id; }
        ))->unbind();
    }

    
    void shaderManger::setUniform1i(shaderId id, const std::string& name, int value)
    {
        (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return id == i->rendererID; }
        ))->setUniform1i(name, value);
    }
    
    void shaderManger::setUniform1f(shaderId id, const std::string& name, float value)
    {
        (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return id == i->rendererID; }
        ))->setUniform1f(name, value);
    }
    
    void shaderManger::setUniform4f(shaderId id, const std::string& name, float v0, float v1, float v2, float v3)
    {
        (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return id == i->rendererID; }
        ))->setUniform4f(name, v0, v1, v2, v3);
    }
    
    void shaderManger::setUniformMat4f(shaderId id, const std::string& name,  void *value)
    {
        (*std::find_if(
            shaders->begin(),
            shaders->end(),
            [=](shader *i) -> bool { return id == i->rendererID; }
        ))->setUniformMat4f(name, value);
    }

}
