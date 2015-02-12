#ifndef _BASIC_LIGHTING_H_
#define _BASIC_LIGHTING_H_

#include "Application.h"
#include "Vertex.h"


class Camera;
class FlyCamera;

class FBXFile;


class BasicLighting : public Application
{
private:
	float m_fTimer;
	float m_fPreviousTime;
	float m_fdeltaTime;

	FBXFile* m_fbx_file;


	// Textures // 
	unsigned int m_Texture;
	// Vertex Array Object
	unsigned int m_VAO;
	// Vertex Buffer Object
	unsigned int m_VBO;
	// Index Buffer Object
	unsigned int m_IBO;
	// Combined Program Object, Contains m_VBO/m_IBO etc
	unsigned int m_ProgramID;

	// Shaders // 
	const char* vertexShader_Source;
	const char* fragmentShader_Source;

	unsigned int m_index_count;

public:
	virtual ~BasicLighting();

	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	// BasicLighting functions
	void createOpenGLBuffers(FBXFile* a_file);
	void cleanupOpenGLBuffers(FBXFile* a_file);
	void reloadShader();


	// Non BasicLighting functions
	virtual bool loadShaders();
	void loadTexture(const char* a_fileName);
	void generateQuad(float a_size);
	void generateGrid(unsigned int rows, unsigned int cols);
};

#endif // _BASIC_LIGHTING_H_