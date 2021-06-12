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
            static materialId nextId;
        public:
            static void init();
            static void close();

            static materialId addMatrial(material *toAdd);
            static void bind(materialId id, shader *shaderToBindTo, std::vector<uint32_t> textureSlots = {});
            static material *getMaterial(materialId id);
    };
    

}