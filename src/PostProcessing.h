#ifndef _POST_PROCESSING_H
#define _POST_PROCESSING_H

#include "Application.h"
#include "stb_image.h"

#include "RenderTarget.h"

#include "Entity.h"

class PostProcessing : public Application
{
private:
	// Base Variables

	// Project Specific
	unsigned int m_diffuse_texture;
	unsigned int m_normal_texture;
	unsigned int m_specular_texture;

	RenderTarget frameBuffer;
	Entity* cube;

	unsigned int post_programID;

public:
	virtual ~PostProcessing();

	// Base Functions 
	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual	void DebugDraw();

	// Project Specific
	

	void LoadTextures();

	void ReloadShaders();

	OpenGLData GeneratePlane(float a_Width, float a_Height);
	OpenGLData GeneratePlane();
};

#endif // _POST_PROCESSING_H