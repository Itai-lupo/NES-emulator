#include "coreWindow.h"
#include "vertexArrayManger.h"
#include "VertexBufferManger.h"
#include "indexBufferManger.h"
#include "shaderManger.h"
#include "orthographicCameraControler.h"

namespace LTE {

    coreWindow::coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui, renderAPI renderAPIType)
        :Title(title), Width(width), Height(height), useImGui(useImGui), renderAPIType(renderAPIType)
    {
        vaManger = new vertexArrayManger();
        vbManger = new VertexBufferManger();
        ibManger = new indexBufferManger();
        sManger = new shaderManger();
        LAUGHTALE_ENGINR_LOG_INFO(id)
    }

}