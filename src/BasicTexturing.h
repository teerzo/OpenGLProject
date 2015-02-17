#ifndef _BASIC_TEXTURING_H_
#define _BASIC_TEXTURING_H_

#include "Application.h"
#include "Vertex.h"


class Camera;
class FlyCamera;

class BasicTexturing : public Application
{
private:
	
	// Project Specific
	OpenGLData m_gl_data;


	// Textures // 
	unsigned int m_Texture;
	

	// Shaders // 
	const char* vertexShader_Source;
	const char* fragmentShader_Source;


public:
	virtual ~BasicTexturing();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	virtual bool loadShaders();
	void loadTexture(const char* a_fileName);
	void generateQuad(float a_size);
	void generateGrid(unsigned int rows, unsigned int cols);
};

#endif // _BASIC_TEXTURING_H_
