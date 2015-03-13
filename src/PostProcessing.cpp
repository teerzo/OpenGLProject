#include "PostProcessing.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

PostProcessing::~PostProcessing()
{

}

void PostProcessing::setDefaults()
{
	this->AppName = "PostProcessing";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool PostProcessing::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Gizmos::create();
	glfwSetTime(0.0);

	LoadShader((GLuint*)&m_reflected_program, "../data/shaders/reflected_vertex.glsl", "../data/shaders/reflected_fragment.glsl", nullptr);

	m_Render_Target.SetWindowSize(1280, 720);
	m_Render_Target.SetPlaneSize(1280, 720);
	m_Render_Target.GenerateFrameBuffer();
	m_Render_Target.m_plane = GeneratePlane(m_Render_Target.m_Width, m_Render_Target.m_Height);

	return true;
}

void PostProcessing::shutdown()
{
	Application::shutdown();
}

bool PostProcessing::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////



	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void PostProcessing::draw()
{
	glClearColor(m_BackGroundColor.x, m_BackGroundColor.y, m_BackGroundColor.z, m_BackGroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());

	}
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////



	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

OpenGLData PostProcessing::GeneratePlane(float a_Width, float a_Height)
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

OpenGLData PostProcessing::GeneratePlane()
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


