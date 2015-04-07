#ifndef _SHADOWS_APP_H_
#define _SHADOWS_APP_H_

#include "Application.h"
#include "tiny_obj_loader.h"
#include "RenderTarget.h"


class ShadowsApp : public Application
{
public:
	std::vector<OpenGLData> glData;
	OpenGLData glPlane;

	GLuint shadowsProgramID;
	GLuint shadowMapProgramID;

	RenderTarget shadowFrameBuffer;

	virtual ~ShadowsApp();

	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual void DebugDraw();

	void ReloadShaders();
	void LoadShaders();

	OpenGLData GeneratePlane();
	void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes);

private:
};

#endif // _TEMPLATEAPPLICATION_H_