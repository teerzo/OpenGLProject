#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
#include "gl_core_4_4.h"

//typedef unsigned int GLuint;
bool LoadShader(GLuint* Program, char* vertex_filename, char* fragment_filename );
bool LoadShader(GLuint* Program, char* vertex_filename, char* fragment_filename, char* geometry_filename );
bool LoadShader(std::string filename, GLuint* Program);
bool LoadShaderType(char* filename, GLenum shader_type, unsigned int* output);

#endif // _UTILITY_H_