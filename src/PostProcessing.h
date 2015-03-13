#ifndef _POST_PROCESSING_H
#define _POST_PROCESSING_H

#include "Application.h"


#include "RenderTarget.h"

class PostProcessing : public Application
{
private:
	// Base Variables

	// Project Specific
	RenderTarget m_Render_Target;

	unsigned int m_reflected_program;

public:
	virtual ~PostProcessing();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	OpenGLData GeneratePlane(float a_Width, float a_Height);
	OpenGLData GeneratePlane();
};

#endif // _POST_PROCESSING_H