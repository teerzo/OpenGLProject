#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>

typedef unsigned int GLuint;
bool LoadShader(char* vertex_filname, char* fragment_filename, GLuint* Program);
bool LoadShader(std::string filename, GLuint* Program);


#endif // _UTILITY_H_