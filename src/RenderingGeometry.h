#ifndef _RENDERING_GEOMETRY_H_
#define _RENDERING_GEOMETRY_H_

#include "Application.h"

class RenderingGeometry : public Application
{
private:
	OpenGLData m_gl_data;
	
	// Shaders // 
	const char* vertexShader_Source;
	const char* fragmentShader_Source;


public:
	virtual ~RenderingGeometry();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	virtual bool loadShaders();
	
	void generateGrid(unsigned int rows, unsigned int cols);
	void generateShader();
};

#endif // _RENDERING_GEOMETRY_H_
