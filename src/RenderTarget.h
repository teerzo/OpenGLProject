#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "Vertex.h"
#include "Utility.h"
#include "Camera.h"

class RenderTarget 
{
public:
	RenderTarget(Screen screenres);
	
	void Render();
	void Draw(GLuint* a_ProgramID);
	void SetWindowSize(Screen screenres);
	void SetPlaneSize(Screen screenres);

	void GenerateFrameBuffer();
	void GenerateShadowFrameBuffer();
	void GenerateGBuffer();
	void GenerateLightBuffer();

	void GeneratePlane();
	void GenerateTarget();
	
	glm::mat4 projection_view;

	Camera* camera;
	Screen screen;
	OpenGLData m_plane;

	float m_Width;
	float m_Height;

	unsigned int gbuffer_fbo;
	unsigned int gbuffer_texture;
	unsigned int gbuffer_depth;

	unsigned int texture_albedo;
	unsigned int texture_position;
	unsigned int texture_normal;

	unsigned int light_buffer_fbo;
	unsigned int light_buffer_texture;
};



#endif // _RENDER_TARGET_H_