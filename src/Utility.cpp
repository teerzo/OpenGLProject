#include "Utility.h"

#include "gl_core_4_4.h"
#include <cstdio>


bool LoadShader(char* vertex_filename, char* fragment_filename, GLuint* Program )
{
	bool result = true;

	FILE* vertex_file = fopen(vertex_filename, "r");
	FILE* fragment_file = fopen(fragment_filename, "r");
	if (vertex_file == nullptr || fragment_file == nullptr)
	{
		return false;
	}
	else if (vertex_file == 0 || fragment_file == 0)
	{	
		result = false;
	}
	else
	{
		// reads in vertex and fragment files
		fseek(vertex_file, 0, SEEK_END);
		int vertex_file_length = ftell(vertex_file);
		fseek(vertex_file, 0, SEEK_SET);

		fseek(fragment_file, 0, SEEK_END);
		int fragment_file_length = ftell(fragment_file);
		fseek(fragment_file, 0, SEEK_SET);

		char* vs_source = new char[vertex_file_length];
		char* fs_source = new char[fragment_file_length];
		
		vertex_file_length = fread(vs_source, 1, vertex_file_length, vertex_file);
		fragment_file_length = fread(fs_source, 1, fragment_file_length, fragment_file);

		// $$ Shader Load code here
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vs_source, &vertex_file_length);
		glCompileShader(vertex_shader);
		glShaderSource(fragment_shader, 1, &fs_source, &fragment_file_length);
		glCompileShader(fragment_shader);

		*Program = glCreateProgram();
		glAttachShader(*Program, vertex_shader);
		glAttachShader(*Program, fragment_shader);
		glLinkProgram(*Program);

		// Error Checking
		int success = GL_FALSE;
		glGetProgramiv(*Program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int log_length = 0;
			glGetProgramiv(*Program, GL_INFO_LOG_LENGTH, &log_length);
			char* log = new char[log_length];
			glGetProgramInfoLog(*Program, log_length, 0, log);
			printf("Error: loadShaders() failed\n ");
			printf("%s", log);

			delete[] log;
		}

		glDeleteShader(fragment_shader);
		glDeleteShader(vertex_shader);
		// $$ end of shader code here 

		delete[] vs_source;
		delete[] fs_source;
	}

	fclose(vertex_file);
	fclose(fragment_file);

	return result;
}