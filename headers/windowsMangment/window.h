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
#include "coreCamera.h"

#if defined(_WIN32) || defined(_WIN64) // Windows

#elif defined(__ANDROID__)  // Android (implies Linux, so it must come first)

#elif defined(__linux__) // linux
    #include "linuxWindow.h"
    #include "linuxInput.h"
    typedef LaughTaleEngine::linuxWindow window;
    typedef LaughTaleEngine::linuxInput input;
    typedef GLFWwindow* windowPtr;
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #if  TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1// Apple iOS
    
    #elif TARGET_OS_MAC == 1 // Apple OSX
    
    #endif
#else
    #error "platform not supported"
#endif

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
            static void onUpdate(IEntity *eventEntity, IEventData *sendor);

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

            static VertexBufferManger *getVertexBufferManger(windowPieceId windowId);
            static vertexArrayManger *getVertexArrayManger(windowPieceId windowId);

            static uint32_t getIndexBufferCount(windowPieceId windowId, indexBufferId id);

            static renderApi *getRenderApi(windowPieceId windowId);
            static renderer *getRenderer(windowPieceId windowId);
            static void setCamera(windowPieceId windowId, coreCamera *cam);
    };
}
    
