
#include "vertexArrayManger.h"
#include "VertexBufferManger.h"
#include "indexBufferManger.h"
#include "shaderManger.h"

namespace LaughTaleEngine {

    coreWindow::coreWindow(const std::string& title, unsigned int width, unsigned int height, bool useImGui)
        :Title(title), Width(width), Height(height), useImGui(useImGui)
    {
        vaManger = new vertexArrayManger();
        vbManger = new VertexBufferManger();
        ibManger = new indexBufferManger();
        sManger = new shaderManger();
    }

}