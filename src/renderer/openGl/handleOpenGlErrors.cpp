#include <iostream>
#include <string>

using namespace std;

#include "glad/glad.h"
#include "handleOpenGlErrors.h"
#include "logger.h"

void GLClearErrors()
{
    while(glGetError() != GL_NO_ERROR);
}

void GLCheckError(const char *function, const char *file, int line)
{
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
        LAUGHTALE_ENGINR_LOG_INFO(
            "[OpenGL error] (" + to_string(error) + "): " + function + " " + file +  ":" + to_string(line) );
    
}
