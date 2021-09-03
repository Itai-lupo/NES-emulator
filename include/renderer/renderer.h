#pragma once
#include "window.h"
#include "renderApi.h"
#include "vertexArray.h"
#include "shader.h"
#include "coreCamera.h"
#include "mesh.h"
#include "scene.h"


namespace LTE
{
    class renderer
    {
        private:
            static inline scene *Scene;
            static inline renderApi *renderPipLine;
        public:                
            static void renderScene(scene *Scene, renderApi *renderPipLine);
    };
    
    
}
