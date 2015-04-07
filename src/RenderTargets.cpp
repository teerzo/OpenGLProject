#include "RenderTargets.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"
#include "Utility.h"

RenderTargets::~RenderTargets()
{

}

void RenderTargets::setDefaults()
{
	this->applicationName = "RenderTargets";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool RenderTargets::ApplicationStartup()
{
	if (!Application::ApplicationStartup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Gizmos::create();
	glfwSetTime(0.0);

	LoadShader((GLuint*)&m_reflected_program, "../data/shaders/reflected_vertex.glsl", "../data/shaders/reflected_fragment.glsl", nullptr);
	LoadShader((GLuint*)&m_ProgramID, "../data/shaders/textured_vertex.glsl", "../data/shaders/textured_fragment.glsl", nullptr );

	m_Front_Target.SetWindowSize(1280, 720);
	m_Front_Target.SetPlaneSize(1280, 720);
	m_Front_Target.GenerateFrameBuffer();
	m_Front_Target.m_plane = GeneratePlane(m_Front_Target.m_Width, m_Front_Target.m_Height );

	return true;
}

void RenderTargets::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool RenderTargets::Update()
{
	if (!Application::Update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////


	Gizmos::addSphere(vec3(0, 5, 0), 0.5f, 12, 12, color.Red);

	m_Front_Target.Update(deltaTime, cameraVector[0]);


	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void RenderTargets::draw()
{
	glUseProgram(m_reflected_program);
	
	// Render()
	glBindFramebuffer(GL_FRAMEBUFFER, m_Front_Target.m_fbo);
	glViewport(0, 0, m_Front_Target.m_screen.Width, m_Front_Target.m_screen.Height);

	glClearColor(0.4f, 0.4f, 0.4f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_DrawGrid();
	_DrawLines();

	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	Gizmos::draw(m_Front_Target.m_Camera->m_mProjectionViewTransform);
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	glViewport(0, 0, ScreenSize.Width, ScreenSize.Height);

	glClearColor(m_BackGroundColor.x, m_BackGroundColor.y, m_BackGroundColor.z, m_BackGroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	int proj_view_handle = glGetUniformLocation(m_reflected_program, "projection_view");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());

	}
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////
	//m_Front_Target.Draw(m_ProgramID);


	// m_Front_Target.Draw(m_reflected_program);
	int reflected_uniform = glGetUniformLocation(m_reflected_program, "reflected_projection_view");
	glUniformMatrix4fv(reflected_uniform, 1, GL_FALSE, (float*)&m_Front_Target.m_Camera->m_mProjectionViewTransform);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Front_Target.m_fbo_texture);
	glUniform1i(glGetUniformLocation(m_reflected_program, "diffuse"), 0);

	glBindVertexArray(m_Front_Target.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, m_Front_Target.m_plane.m_index_count, GL_UNSIGNED_INT, 0);
	// 
	



	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

OpenGLData RenderTargets::GeneratePlane(float a_Width, float a_Height)
{
	OpenGLData temp;
	temp.m_index_count = 6;
	float vertexData[] = {
		-a_Width / 100, 0, -5.1, 1, 0, 0,
		a_Width / 100, 0, -5.1, 1, 1, 0,
		a_Width / 100, a_Height / 100, -5.1, 1, 1, 1,
		-a_Width / 100, a_Height / 100, -5.1, 1, 0, 1,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* temp.m_index_count * 4, vertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* temp.m_index_count, indexData, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position 
	glEnableVertexAttribArray(1); // UV
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, 0); // Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, ((char*)0) + 16); // UV

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return temp;
}

OpenGLData RenderTargets::GeneratePlane()
{
	OpenGLData temp;
	temp.m_index_count = 6;
	float vertexData[] = {
		-5, 0, -5, 1, 0, 0,
		5, 0, -5, 1, 1, 0,
		5, 10, -5, 1, 1, 1,
		-5, 10, -5, 1, 0, 1,
	};
	unsigned int indexData[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* temp.m_index_count * 4,vertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* temp.m_index_count,	indexData, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position 
	glEnableVertexAttribArray(1); // UV
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, 0); // Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,	sizeof(float)* temp.m_index_count, ((char*)0) + 16); // UV

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return temp;
}