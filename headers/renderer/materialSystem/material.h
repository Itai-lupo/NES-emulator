#pragma once
#include "core.h"
#include "shader.h"
#include "materialsManger.h"

namespace LaughTaleEngine
{
    class material
    {
    private:
        friend materialsManger;
        matrialId id;

    public:
        virtual ~material() = default;
        virtual void bind(shader *s) = 0;
    };
}