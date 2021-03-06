#include "Utility.h"

#include "gl_core_4_4.h"
#include <cstdio>
#include "stb_image.h"


bool LoadShaderType(char* filename, GLenum shader_type, unsigned int* output)
{
	bool result = true;

	FILE* shader_file = fopen(filename, "r");
	if (shader_file == nullptr)
	{
		result = false;
	}
	else if (shader_file == 0)
	{
		result = false;
	}
	else
	{
		// find file length
		fseek(shader_file, 0, SEEK_END);
		int shader_file_length = ftell(shader_file);
		fseek(shader_file, 0, SEEK_SET);

		// allocate space for file
		char* shader_source = new char[shader_file_length];
		// read the file and update the length
		shader_file_length = fread(shader_source, 1, shader_file_length, shader_file);
		// create the shader based on the type passed in 
		unsigned int shader = glCreateShader(shader_type);
		// compile shader
		glShaderSource(shader, 1, &shader_source, &shader_file_length);
		glCompileShader(shader);

		// Error Checking
		int success = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			int log_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
			char* log = new char[log_length];
			glGetShaderInfoLog(shader, log_length, 0, log);
			printf("Error: loadShaders() failed\n ");
			printf("%s", log);
			result = false;
			delete[] log;
		}
		*output = shader;

		// $$ end of shader code here 

		// Causes the shader to be empty before linking
		//delete[] shader_source;
	}

	fclose(shader_file);

	return result;

}

bool LoadShader(GLuint* Program, char* vertex_filename, char* fragment_filename)
{
	bool result = true;

	unsigned int vertex_shader;
	unsigned int fragment_shader;
	LoadShaderType(vertex_filename, GL_VERTEX_SHADER, &vertex_shader);
	LoadShaderType(fragment_filename, GL_FRAGMENT_SHADER, &fragment_shader);

	*Program = glCreateProgram();
	// attach shader to program
	glAttachShader(*Program, vertex_shader);
	glAttachShader(*Program, fragment_shader);
	// link shaders to program 
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

	return result;
}

bool LoadShader(std::string filename, GLuint* Program)
{
	bool result = true;

	std::string vsName = "../data/shaders/" + filename + "_vertex.glsl";
	std::string fsName = "../data/shaders/" + filename + "_fragment.glsl";

	FILE* vertex_file = fopen(vsName.c_str(), "r");
	FILE* fragment_file = fopen(fsName.c_str(), "r");
	if (vertex_file == nullptr || fragment_file == nullptr)
	{
		return false;
		printf("Shader: \"%s\" Failed to Load, Path Incorrect\n", filename.c_str());
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

		// declare opengl shader containers
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		// give shader source code to opengl containers
		glShaderSource(vertex_shader, 1, &vs_source, &vertex_file_length);
		// compile shader code 
		glCompileShader(vertex_shader);

		glShaderSource(fragment_shader, 1, &fs_source, &fragment_file_length);
		glCompileShader(fragment_shader);

		*Program = glCreateProgram();
		// attach shader to program
		glAttachShader(*Program, vertex_shader);
		glAttachShader(*Program, fragment_shader);
		// link shaders to program 
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
		printf("Shader: \"%s\" Successfully Loaded\n", filename.c_str());

		delete[] vs_source;
		delete[] fs_source;
	}

	fclose(vertex_file);
	fclose(fragment_file);

	return result;
}

//bool LoadShader( GLuint* Program, char* vertex_filename, char* fragment_filename = nullptr, char* geometry_filename = nullptr ) {
//	bool result = true;
//	*Program = glCreateProgram();
//	if( vertex_filename != nullptr ) {
//		unsigned int vertex_shader;
//		if( LoadShaderType( vertex_filename, GL_VERTEX_SHADER, &vertex_shader ) ) {
//			glAttachShader( *Program, vertex_shader );
//			glDeleteShader( vertex_shader );
//		}
//	}
//	if( fragment_filename != nullptr ) {
//		unsigned int fragment_shader;
//		if( LoadShaderType( fragment_filename, GL_FRAGMENT_SHADER, &fragment_shader ) ) {
//			glAttachShader( *Program, fragment_shader );
//			glDeleteShader( fragment_shader );
//		}
//	}
//	if( geometry_filename != nullptr ) {
//		unsigned int geometry_shader;
//		if( LoadShaderType( geometry_filename, GL_GEOMETRY_SHADER, &geometry_shader ) ) {
//			glAttachShader( *Program, geometry_shader );
//			glDeleteShader( geometry_shader );
//		}
//	}
//	// link shaders to program 
//	glLinkProgram( *Program );
//
//	// Error Checking
//	int success = GL_FALSE;
//	glGetProgramiv( *Program, GL_LINK_STATUS, &success );
//	if( success == GL_FALSE ) {
//		int log_length = 0;
//		glGetProgramiv( *Program, GL_INFO_LOG_LENGTH, &log_length );
//		char* log = new char[log_length];
//		glGetProgramInfoLog( *Program, log_length, 0, log );
//		printf( "Error: GPU SHIT FAILED \n " );
//		printf( "%s", log );
//		result = false;
//
//		delete[] log;
//	}
//
//	return result;
//}


bool LoadShader( GLuint* Program, char* vertex_filename = nullptr, char* fragment_filename = nullptr, char* geometry_filename = nullptr )
{
	bool result = true;
	*Program = glCreateProgram();
	if ( vertex_filename != nullptr ) {
	
		std::string vertex_path = "../data/shaders/" + (std::string)vertex_filename;
		unsigned int vertex_shader;
		if( LoadShaderType( (char*)vertex_path.c_str(), GL_VERTEX_SHADER, &vertex_shader ) )
		{
			glAttachShader(*Program, vertex_shader);
			glDeleteShader(vertex_shader);
		}
	}
	if( fragment_filename != nullptr ) {
		std::string fragment_path = "../data/shaders/" + ( std::string )fragment_filename;
		unsigned int fragment_shader;
		if( LoadShaderType( (char*)fragment_path.c_str(), GL_FRAGMENT_SHADER, &fragment_shader ) )
		{
			glAttachShader(*Program, fragment_shader);
			glDeleteShader(fragment_shader);
		}
	}
	if( geometry_filename != nullptr ) {
		std::string geometry_path = "../data/shaders/" + ( std::string )geometry_filename;
		unsigned int geometry_shader;
		if( LoadShaderType( ( char* ) geometry_path.c_str() , GL_GEOMETRY_SHADER, &geometry_shader ) )
		{
			glAttachShader(*Program, geometry_shader);
			glDeleteShader(geometry_shader);
		}
	}
	// link shaders to program 
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
		printf("Error: Shaders SHIT FAILED \n ");
		printf("%s", log);
		result = false;

		delete[] log;
	}

	return result;
}

bool LoadTexture( unsigned int* textureID, const char* filename)
{
	bool result = true;
	int width;
	int height;

	int channels;

	std::string texture_path = "../data/textures/" + (std::string)filename;

	unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &channels, STBI_default);

	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(data);
	return result;
}