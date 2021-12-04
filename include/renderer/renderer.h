#pragma once
#include "window.h"
#include "renderApi.h"
#include "vertexArray.h"
#include "shader.h"
#include "coreCamera.h"
#include "mesh.h"
#include "scene.h"
#include "shaderRenderBuffer.h"


namespace LTE
{
    class renderer
    {
        private:
            // static inline window *win;
            // static inline scene *Scene;
            // static inline renderApi *renderPipLine;
        public:                
            static shaderRenderBuffer *submitShape(mesh *shape, material *shapeMatrial);

            static void renderScene(windowPieceId winId, renderApi *renderPipLine);
    };
    
    
}
