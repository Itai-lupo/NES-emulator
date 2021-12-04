#include "glslFile.h"
#include "shaderRenderBuffer.h"
#include "windowManger.h"
#include "logger.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


namespace LTE
{

    glslFile::glslFile(const std::string& filePath): file(filePath){}

    glslFile::~glslFile(){}

    asset *glslFile::genrateAssetForWindow(windowPieceId winId)
    {
        shader *s = windowManger::getWindow(winId)->context->getMeshFactory()->createShader(source.VertexSource, source.FragmentSource);
        return new shaderRenderBuffer(s, winId);
    }

    void glslFile::loadFileData()
    {
        ParseShaders();
    }


    void glslFile::ParseShaders()
    {
        std::ifstream stream(filePath);

        enum ShaderType
        {
            NONE = -1,
            vertex = 0,
            fragment = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;

        while (getline(stream, line)){
            if(line.find("#shader") != std::string::npos)
            {
                type =  (ShaderType)
                        ((line.find("vertex") != std::string::npos) * ((int)ShaderType::vertex - (int)ShaderType::NONE) +
                        (line.find("fragment") != std::string::npos) * ((int)ShaderType::fragment - (int)ShaderType::NONE) +
                        (int)ShaderType::NONE);
            }
            else if((int)type >= 0)
            {
                ss[(int)type] << line << "\n";
            }
        }

        source.VertexSource = ss[(int)ShaderType::vertex].str();
        source.FragmentSource = ss[(int)ShaderType::fragment].str();
    }
}