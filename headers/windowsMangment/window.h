#pragma once
#include <string>
#include <functional>
#include <vector>

#include "events.h"

#include "core.h"
#include "shader.h"
#include "shaderManger.h"
#include "vertexArray.h"
#include "vertexArrayManger.h"
#include "VertexBuffer.h"
#include "VertexBufferManger.h"
#include "indexBufferManger.h"
#include "renderApi.h"
#include "renderer.h"

namespace LaughTaleEngine
{
    class windowManger
    {
        private:
            static bool VSync;
        public:
            static void init();
            static void close();
            
            static windowPieceId addWindow(const std::string& title = "raftel engine", bool useImGui = false, unsigned int width = 1280, unsigned int height = 720, renderAPI renderAPIType = renderAPI::OpenGL);
            static void onUpdate(IEventData *sendor);

            static window *getWindow(windowPieceId windowId);

            static unsigned int getWidth(windowPieceId windowId);
            static unsigned int getHeight(windowPieceId windowId);

            static windowPtr raftelIdToWindowReference(windowPieceId windowId);

            static void setVSync(bool enabled);

            static bool isVSync(){ return VSync; };

            static void bindContext(windowPieceId windowId);


            static vertexBufferId add(windowPieceId windowId, VertexBuffer *data);
            static indexBufferId add(windowPieceId windowId, indexBuffer *data);
            static vertexArrayId add(windowPieceId windowId, vertexArray *data);
            static shaderId add(windowPieceId windowId, shader *data);

            static void bindVB(windowPieceId windowId, vertexBufferId id);
            static void bindIB(windowPieceId windowId, indexBufferId id);
            static void bindVA(windowPieceId windowId, vertexArrayId id);
            static void bindS(windowPieceId windowId, shaderId id);

            static void pushElement(windowPieceId windowId, vertexBufferId id, VertexBufferElement data);
            static void addBuffer(windowPieceId windowId, vertexArrayId id, vertexBufferId vbId);

            static shaderManger *getShaderManger(windowPieceId windowId);
            static renderApi *getRenderApi(windowPieceId windowId);
            static renderer *getRenderer(windowPieceId windowId);
    };
}
    
