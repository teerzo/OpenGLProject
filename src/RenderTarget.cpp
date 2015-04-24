#include "RenderTarget.h"
#include "Gizmos.h"

RenderTarget::RenderTarget(Screen screenres)
{
	//m_Camera = new Camera(screenres);
	SetWindowSize(screenres);
	SetPlaneSize(screenres);
	GenerateTarget();
	GenerateGBuffer();
	GenerateLightBuffer();
}

void RenderTarget::SetWindowSize(Screen screenres)
{
	screen = screenres;
}

void RenderTarget::SetPlaneSize(Screen screenres)
{
	m_Width = (float)screenres.width;
	m_Height = (float)screenres.height;
}

//void RenderTarget::Update(float a_DeltaTime, Camera* a_Camera)
//{
//	glm::vec4 plane = glm::vec4(0, 0, 1, -5);
//
//	m_Camera->matrix_world = a_Camera->GetWorld();
//
//	glm::vec3 reflected = glm::reflect(-m_Camera->matrix_world[2].xyz(), plane.xyz());
//	reflected = glm::normalize(reflected);
//
//	float distance = (glm::dot((plane.xyz() * plane.w) - m_Camera->matrix_world[3].xyz(), plane.xyz())) / (glm::dot(plane.xyz(), -m_Camera->matrix_world[2].xyz()));
//
//	glm::vec3 inter = m_Camera->matrix_world[3].xyz() + (-m_Camera->matrix_world[2].xyz()) * distance;
//
//	m_Camera->matrix_world[3].xyz = inter - reflected * distance;
//
//	glm::vec3 up = glm::vec3(0, 1, 0);
//	m_Camera->matrix_world[2].xyz = -reflected;
//	m_Camera->matrix_world[0].xyz = glm::normalize(glm::cross(m_Camera->matrix_world[2].xyz(), up));
//	m_Camera->matrix_world[1].xyz = glm::normalize(glm::cross(m_Camera->matrix_world[0].xyz(), m_Camera->matrix_world[2].xyz()));
//	m_Camera->matrix_view = glm::inverse(m_Camera->matrix_world);
//	m_Camera->matrix_projection_view = a_Camera->GetProjection() * m_Camera->matrix_view;
//
//	//vec3 temp = proj_view[3].xyz;
//	//m_Camera->setPosition((-temp));
//	m_Camera->Update(a_DeltaTime);
//}

void RenderTarget::Render()
{


}


void RenderTarget::Draw(GLuint* a_ProgramID)
{


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer_texture);
	glUniform1i(glGetUniformLocation(*a_ProgramID, "input_texture"), 0);

	glBindVertexArray(m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}


void RenderTarget::GenerateFrameBuffer()
{
	glGenFramebuffers(1, &gbuffer_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);

	glGenTextures(1, &gbuffer_texture);
	glBindTexture(GL_TEXTURE_2D, gbuffer_texture);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, screen.width, screen.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, gbuffer_texture, 0);

	glGenRenderbuffers(1, &gbuffer_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, gbuffer_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, screen.width, screen.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gbuffer_depth);

	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderTarget::GenerateShadowFrameBuffer()
{
	// setup shadow map buffer
	glGenFramebuffers(1, &gbuffer_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	glGenTextures(1, &gbuffer_depth);
	glBindTexture(GL_TEXTURE_2D, gbuffer_depth);
	// texture uses a 16-bit depth component format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, screen.width, screen.height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);
	// attached as a depth attachment to capture depth not colour
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		gbuffer_depth, 0);
	// no colour targets are used
	glDrawBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::GenerateGBuffer()
{
	// create the Framebuffer
	glGenFramebuffers(1, &gbuffer_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	// generate textures
	glGenTextures(1, &texture_albedo);
	glBindTexture(GL_TEXTURE_2D, texture_albedo);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, screen.width, screen.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &texture_position);
	glBindTexture(GL_TEXTURE_2D, texture_position);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, screen.width, screen.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &texture_normal);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, screen.width, screen.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &gbuffer_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, gbuffer_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen.width, screen.height);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_albedo, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, texture_position, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, texture_normal, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gbuffer_depth);

	GLenum gpassTargets[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, gpassTargets);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::GenerateLightBuffer()
{
	glGenFramebuffers(1, &light_buffer_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, light_buffer_fbo);
	glGenTextures(1, &light_buffer_texture);
	glBindTexture(GL_TEXTURE_2D, light_buffer_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, screen.width, screen.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, light_buffer_texture, 0);
	GLenum lightTargets[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightTargets);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::GeneratePlane()
{

}

void RenderTarget::GenerateTarget()
{
	m_plane.m_index_count = 6;
	float vertexData[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &m_plane.m_VAO);
	glBindVertexArray(m_plane.m_VAO);
	glGenBuffers(1, &m_plane.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_plane.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* m_plane.m_index_count * 4, vertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &m_plane.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* m_plane.m_index_count, indexData, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position 
	glEnableVertexAttribArray(1); // UV
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* m_plane.m_index_count, 0); // Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* m_plane.m_index_count, ((char*)0) + 16); // UV

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}