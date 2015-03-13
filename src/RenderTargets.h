#ifndef _RENDER_TARGETS_H_
#define _RENDER_TARGETS_H_

#include "Application.h"
#include "Mesh.h"

#include "RenderTarget.h"

class RenderTargets : public Application
{
private:
	// Base Variables
	RenderTarget m_Front_Target;
	
	unsigned int m_reflected_program;

	// Project Specific
	//Screen m_screen;
	//OpenGLData m_plane;
	//OpenGLData m_reverse;

	//unsigned int m_fbo; // frame buffer object
	//unsigned int m_fbo_texture; // 
	//unsigned int m_fbo_depth;


public:
	virtual ~RenderTargets();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	//void GenerateFrameBuffer();
	OpenGLData GeneratePlane( float a_Width, float a_Height); 
	OpenGLData GeneratePlane();
};

#endif // _RENDER_TARGETS_H_