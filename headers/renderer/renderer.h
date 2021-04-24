#pragma once
#include "renderApi.h"
#include "vertexArray.h"
#include "shader.h"

namespace LaughTaleEngine
{
    class renderer
    {
        private:
            renderApi *api;
        public:
            renderer(renderApi *api):
                api(api){}
                
            void BeginScene();
            void EndScene();

            void Submit(uint32_t count);
    };
    
    
}
