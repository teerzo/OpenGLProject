#include "RenderTarget.h"
#include "Gizmos.h"

RenderTarget::RenderTarget()
{
	m_Camera = new RenderTargetCamera();
	
}

void RenderTarget::SetWindowSize(int a_Width, int a_Height)
{
	m_screen.Width = a_Width;
	m_screen.Height = a_Height;
}

void RenderTarget::SetPlaneSize(int a_Width, int a_Height)
{
	m_Width = a_Width;
	m_Height = a_Height;
}

void RenderTarget::Update(float a_DeltaTime, Camera* a_Camera )
{
	vec4 plane = vec4(0, 0, 1, -5);

	m_Camera->m_mWorldTransform = a_Camera->getWorldTransform();

	vec3 reflected = glm::reflect(-m_Camera->m_mWorldTransform[2].xyz(), plane.xyz());
	reflected = glm::normalize(reflected);

	float distance = (glm::dot((plane.xyz() * plane.w) - m_Camera->m_mWorldTransform[3].xyz(), plane.xyz())) / (glm::dot(plane.xyz(), -m_Camera->m_mWorldTransform[2].xyz()));

	vec3 inter = m_Camera->m_mWorldTransform[3].xyz() + (-m_Camera->m_mWorldTransform[2].xyz()) * distance;

	m_Camera->m_mWorldTransform[3].xyz = inter - reflected * distance;

	vec3 up = vec3(0, 1, 0);
	m_Camera->m_mWorldTransform[2].xyz = -reflected;
	m_Camera->m_mWorldTransform[0].xyz = glm::normalize(glm::cross(m_Camera->m_mWorldTransform[2].xyz(), up));
	m_Camera->m_mWorldTransform[1].xyz = glm::normalize(glm::cross(m_Camera->m_mWorldTransform[0].xyz(), m_Camera->m_mWorldTransform[2].xyz()));
	m_Camera->m_mViewTransform = glm::inverse(m_Camera->m_mWorldTransform);
	m_Camera->m_mProjectionViewTransform = a_Camera->getProjection() * m_Camera->m_mViewTransform;

	//vec3 temp = proj_view[3].xyz;
	//m_Camera->setPosition((-temp));
	m_Camera->update(a_DeltaTime);
}

void RenderTarget::Render()
{

	
}


void RenderTarget::Draw( GLuint a_ProgramID )
{

	

}


void RenderTarget::GenerateFrameBuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fbo_texture);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_screen.Width, m_screen.Height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fbo_texture, 0);

	glGenRenderbuffers(1, &m_fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_screen.Width, m_screen.Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fbo_depth);

	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}