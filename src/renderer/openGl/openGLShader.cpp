#include "openGLShader.h"
#include "glad/glad.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "logger.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "handleOpenGlErrors.h"

namespace LaughTaleEngine
{
    shaderId openGLShader::init()
    {
        ParseShaders();
        createShader();

        GL_CALL(glUseProgram(rendererID));
        return rendererID;
    }

    openGLShader::~openGLShader()
    {
        GL_CALL(glDeleteProgram(rendererID));
    }

    void openGLShader::ParseShaders()
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

    void openGLShader::createShader()
    {
        GL_CALL(rendererID = glCreateProgram());
        
        GL_CALL(unsigned int vs = compileShader(GL_VERTEX_SHADER, source.VertexSource);)
        GL_CALL(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, source.FragmentSource));

        GL_CALL(glAttachShader(rendererID, vs));
        GL_CALL(glAttachShader(rendererID, fs));
        GL_CALL(glLinkProgram(rendererID));
        GL_CALL(glValidateProgram(rendererID));

        GL_CALL(glDeleteShader(vs));
        GL_CALL(glDeleteShader(fs));
    }

    unsigned int openGLShader::compileShader(unsigned int type, const std::string& source)
    {
        unsigned int id;
        GL_CALL(id = glCreateShader(type));
        const char *src = source.c_str();
        GL_CALL(glShaderSource(id, 1, &src, nullptr));
        GL_CALL(glCompileShader(id));

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if(result == GL_FALSE)
        {
            int length;
            GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
            char* message = (char*)alloca(length * sizeof(char));
            GL_CALL(glGetShaderInfoLog(id, length, &length, message));
            LAUGHTALE_ENGINR_LOG_ERROR( 
                std::string("falid to compile ") << 
                std::string( type == GL_VERTEX_SHADER ? "vertex": "fragment" )  <<
                std::string(" shader\n") <<
                std::string(message)
            );
            GL_CALL(glDeleteShader(id));
            return 0;
        }

        return id;
    }



    void openGLShader::bind()
    {
        GL_CALL(glUseProgram(rendererID));
    }

    void openGLShader::unbind()
    {
        GL_CALL(glUseProgram(0));
    }


    void openGLShader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void openGLShader::setUniform1i(const std::string& name, int value)
    {
        GL_CALL(glUniform1i(GetUniformLocation(name), value));
    }

    void openGLShader::setUniform1f(const std::string& name, float value)
    {
        GL_CALL(glUniform1f(GetUniformLocation(name), value));
    }

    void openGLShader::setUniformMat4f(const std::string& name, const glm::mat4& value)
    {   
        GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
    }


    int openGLShader::GetUniformLocation(const std::string& name)
    {
        if(m_UniformLoctionCache.find(name) != m_UniformLoctionCache.end()) 
            return m_UniformLoctionCache[name];

        GL_CALL(m_UniformLoctionCache[name] = glGetUniformLocation(rendererID, name.c_str()));

        return m_UniformLoctionCache[name];
    }


}