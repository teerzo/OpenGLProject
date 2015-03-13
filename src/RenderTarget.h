#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H

#include "Vertex.h"
#include "Utility.h"
#include "Camera.h"

class RenderTarget
{
private:
		
protected:

public:
	RenderTarget();
	
	void Update(float a_DeltaTime, Camera* a_Camera);
	void Render();
	void Draw(GLuint a_ProgramID);
	void SetWindowSize(int a_Width, int a_Height);
	void SetPlaneSize(int a_Width, int a_Height);

	void GenerateFrameBuffer();
	
	mat4 proj_view;

	RenderTargetCamera* m_Camera;
	Screen m_screen;
	OpenGLData m_plane;

	float m_Width;
	float m_Height;

	unsigned int m_fbo;
	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;
};

#endif // _RENDER_TARGET_H__