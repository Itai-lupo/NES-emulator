#pragma once


#define GL_CALL(x) GLClearErrors();\
                    x;\
                    GLCheckError(#x, __FILE__, __LINE__);


void GLClearErrors();

void GLCheckError(const char *function, const char *file, int line);
