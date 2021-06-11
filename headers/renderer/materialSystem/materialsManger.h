#pragma once
#include "core.h"
#include "shader.h"
#include <vector>

namespace LaughTaleEngine
{
    class material;
    class materialsManger
    {
        private:
            static std::vector<material *> *materials;
            static matrialId nextId;
        public:
            static void init();
            static void close();

            static matrialId addMatrial(material *toAdd);
            static void bind(matrialId id, shader *shaderToBindTo);
    };
    

}