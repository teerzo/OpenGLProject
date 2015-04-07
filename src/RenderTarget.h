#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "Vertex.h"
#include "Utility.h"
#include "Camera.h"

class RenderTarget 
{
public:
	RenderTarget();
	
	void Update(float a_DeltaTime, Camera* a_Camera);
	void Render();
	void Draw(GLuint* a_ProgramID);
	void SetWindowSize(int a_Width, int a_Height);
	void SetPlaneSize(int a_Width, int a_Height);

	void GenerateFrameBuffer();
	void GenerateShadowFrameBuffer();
	void GenerateGBuffer();
	void GenerateLightBuffer();

	void GeneratePlane();
	void GenerateTarget();
	
	mat4 proj_view;

	RenderTargetCamera* m_Camera;
	Screen m_screen;
	OpenGLData m_plane;

	float m_Width;
	float m_Height;

	unsigned int m_fbo;
	unsigned int m_fbo_texture;
	unsigned int m_fbo_depth;

	unsigned int albedo_texture;
	unsigned int position_texture;
	unsigned int normals_texture;

	unsigned int light_fbo;
	unsigned int light_texture;
};



#endif // _RENDER_TARGET_H_