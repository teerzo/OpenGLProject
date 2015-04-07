#include "PostProcessing.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

PostProcessing::~PostProcessing()
{

}

void PostProcessing::SetApplicationDefaults()
{
	this->applicationName = "PostProcessing";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool PostProcessing::ApplicationStartup()
{
	if (!Application::ApplicationStartup())
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

	LoadShader((GLuint*)&post_programID, "../data/shaders/post_vertex.glsl", "../data/shaders/post_fragment.glsl", nullptr);
	LoadShader((GLuint*)&m_ProgramID, "../data/shaders/normalmap_vertex.glsl", "../data/shaders/normalmap_fragment.glsl", nullptr);
	LoadTextures();

	frameBuffer.SetWindowSize(1280, 720);
	frameBuffer.SetPlaneSize(1280, 720);
	frameBuffer.m_plane = GeneratePlane(frameBuffer.m_Width, frameBuffer.m_Height);
	frameBuffer.GenerateFrameBuffer();

	cube = new Entity;
	cube->GenerateCube();

	return true;
}

void PostProcessing::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool PostProcessing::Update()
{
	CheckInput();
	if (!Application::Update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	cube->Update();

	//frameBuffer.Update(deltaTime, cameraVector[currentCamera]);

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void PostProcessing::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.m_fbo);
	glViewport(0, 0, frameBuffer.m_screen.Width, frameBuffer.m_screen.Height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ProgramID);
	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());
	}
	glUniform1f(glGetUniformLocation(m_ProgramID, "timer"), currentGameTime);
	glUniform3fv(glGetUniformLocation(m_ProgramID, "eye_pos"),1 ,(float*)&cameraVector[currentCamera]->GetPosition());
	glUniform3fv(glGetUniformLocation(m_ProgramID, "light_dir"), 1, (float*)&cameraVector[0]->GetForward());
	glUniform1f(glGetUniformLocation(m_ProgramID, "specular_power"), 20.0f);
	glUniform3fv(glGetUniformLocation(m_ProgramID, "ambient_light"), 1, (float*)&glm::vec3( 0.2f,0.2f,0.2f) );
	glUniform3fv(glGetUniformLocation(m_ProgramID, "light_color"), 1, (float*)&glm::vec3(1,1,1));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	glUniform1i(glGetUniformLocation(m_ProgramID, "diffuse_tex"), 0);
	glUniform1i(glGetUniformLocation(m_ProgramID, "normal_tex"), 1);
	glUniform1i(glGetUniformLocation(m_ProgramID, "specular_tex"), 2);

	// DRAW CODE
	DebugDraw();
	cube->Draw();

	////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(post_programID);

	//frameBuffer.Draw((GLuint*)&post_programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffer.m_fbo_texture);
	glUniform1i(glGetUniformLocation(post_programID, "input_texture"), 0);
	glUniform1f(glGetUniformLocation(post_programID, "timer"), currentGameTime);
	glBindVertexArray(frameBuffer.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/*int proj_view_handle = glGetUniformLocation(post_programID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	}*/
	

	//glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);

	
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////




	//////////////////////
	//! End of Draw Code 
	//////////////////////
	//Application::DebugDraw();
	Application::Draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}


OpenGLData PostProcessing::GeneratePlane(float a_Width, float a_Height)
{
	OpenGLData temp;
	temp.m_index_count = 6;

	glm::vec2 half_texel = 1.0f / glm::vec2(a_Width, a_Height);

	float vertexData[] = {
		-1, -1,		 0, 1,	half_texel.x, half_texel.y,		// 0, 0,
		-1, 1, 0, 1, half_texel.x, 1.0f - half_texel.y,		// 1, 0,
		1, 1,		 0, 1,	1.0f - half_texel.x, 1.0f - half_texel.y,		// 1, 1,
		1, -1, 0, 1, 1.0f - half_texel.x, half_texel.y		// 0, 1,
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

void PostProcessing::DebugDraw()
{
	Application::DebugDraw();
}

void PostProcessing::CheckInput()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShaders();
	}

	Application::CheckInput();
}

void PostProcessing::LoadTextures()
{
	int width, height, channels;

	unsigned char *data;

	data = stbi_load("../data/textures/four_diffuse.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_diffuse_texture);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	data = stbi_load("../data/textures/four_normal.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_normal_texture);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	data = stbi_load("../data/textures/four_specular.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_specular_texture);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

}

void PostProcessing::ReloadShaders()
{ 
	glfwSetTime(0.0);
	currentGameTime = 0.0f;
	glDeleteProgram(m_ProgramID);
	glDeleteProgram(post_programID);
	printf("reloaded Shaders\n");
	LoadShader((GLuint*)&post_programID, "../data/shaders/post_vertex.glsl", "../data/shaders/post_fragment.glsl", nullptr);
	LoadShader((GLuint*)&m_ProgramID, "../data/shaders/normalmap_vertex.glsl", "../data/shaders/normalmap_fragment.glsl", nullptr);
}