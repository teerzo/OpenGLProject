#include "ShadowsApp.h"

//#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

ShadowsApp::~ShadowsApp()
{

}

void ShadowsApp::SetApplicationDefaults()
{
	this->applicationName = "ShadowsApp";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool ShadowsApp::ApplicationStartup()
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

	// Load Shadow Frame Buffer
	shadowFrameBuffer.SetWindowSize(1280, 720);
	shadowFrameBuffer.SetPlaneSize(1280, 720);
	shadowFrameBuffer.m_plane = GeneratePlane();
	shadowFrameBuffer.GenerateShadowFrameBuffer();

	// Load Plane
	glPlane = GeneratePlane();
	// Load Mesh
	std::vector <tinyobj::shape_t> shapes;
	std::vector <tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, "../data/models/stanford/bunny.obj");
	printf("Mesh load error %s\n", err);
	if (err.size() != 0)
	{

		return false;
	}

	//createOpenGLBuffers(m_fbx_file);
	CreateOpenGLBuffers(shapes);
	// Load Shaders
	LoadShaders();

	return true;
}

void ShadowsApp::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool ShadowsApp::Update()
{
	CheckInput();
	if (!Application::Update())
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

void ShadowsApp::Draw()
{
	// shadow pass: bind our shadow map target and clear the depth
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer.m_fbo);
	glViewport(0, 0, shadowFrameBuffer.m_screen.Width, shadowFrameBuffer.m_screen.Height);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowMapProgramID);

	glUniformMatrix4fv(glGetUniformLocation(shadowMapProgramID, "light_matrix"), 1, GL_FALSE, (float*)&cameraVector[0]->GetProjectionView());
	// Render Mesh
	for (unsigned int mesh_index = 0; mesh_index < glData.size(); ++mesh_index)
	{
		glBindVertexArray(glData[mesh_index].m_VAO);
		glDrawElements(GL_TRIANGLES, glData[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}
	// Draw Plane
	glBindVertexArray(glPlane.m_VAO);
	glDrawElements(GL_TRIANGLES, glPlane.m_index_count, GL_UNSIGNED_INT, 0);


	///// Main Render?
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, shadowFrameBuffer.m_screen.Width, shadowFrameBuffer.m_screen.Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowsProgramID);

	int proj_view_handle = glGetUniformLocation(shadowsProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	}

	glm::mat4 textureSpaceOffset(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
		);

	glm::mat4 LightMatrix = textureSpaceOffset * cameraVector[0]->GetProjectionView();
	glUniformMatrix4fv(glGetUniformLocation(shadowsProgramID, "light_matrix"), 1, GL_FALSE, (float*)&LightMatrix);
	glUniform3fv(glGetUniformLocation(shadowsProgramID, "light_dir"), 1, (float*)&cameraVector[0]->GetForward());
	glUniform1i(glGetUniformLocation(shadowsProgramID, "shadow_map"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowFrameBuffer.m_fbo_depth);
	// Render Mesh
	for (unsigned int mesh_index = 0; mesh_index < glData.size(); ++mesh_index)
	{
		glBindVertexArray(glData[mesh_index].m_VAO);
		glDrawElements(GL_TRIANGLES, glData[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}
	// Draw Plane
	glBindVertexArray(glPlane.m_VAO);
	glDrawElements(GL_TRIANGLES, glPlane.m_index_count, GL_UNSIGNED_INT, 0);

	

	

	//////////////
	/*glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowsProgramID);

	int proj_view_handle = glGetUniformLocation(shadowsProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	}*/
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////


	//////////////////////
	//! End of Draw Code 
	//////////////////////
	DebugDraw();
	Application::Draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void ShadowsApp::DebugDraw()
{
	Application::DebugDraw();
}

void ShadowsApp::CheckInput()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShaders();
	}
	Application::CheckInput();
}

OpenGLData ShadowsApp::GeneratePlane()
{
	OpenGLData temp;
	temp.m_index_count = 6;
	float vertexData[] = {
		-5.0f, 0.1f, -5.0f, 1.0f, 0.0f, 1.0f,
		-5.0f, 0.1f, 5.0f, 1.0f, 0.0f, 1.0f,
		5.0f, 0.1f, 5.0f, 1.0f, 0.0f, 1.0f,
		5.0f, 0.1f, -5.0f, 1.0f, 0.0f, 1.0f,
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

void ShadowsApp::CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes)
{
	glData.resize(a_shapes.size());

	for (unsigned int shape_index = 0; shape_index < a_shapes.size(); ++shape_index)
	{
		std::vector<float> vertex_data;

		unsigned int float_count = a_shapes[shape_index].mesh.positions.size();
		float_count += a_shapes[shape_index].mesh.normals.size();
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.positions.begin(), a_shapes[shape_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.normals.begin(), a_shapes[shape_index].mesh.normals.end());

		glData[shape_index].m_index_count = a_shapes[shape_index].mesh.indices.size();

		// generate ....
		glGenVertexArrays(1, &glData[shape_index].m_VAO);
		glGenBuffers(1, &glData[shape_index].m_VBO);
		glGenBuffers(1, &glData[shape_index].m_IBO);

		glBindVertexArray(glData[shape_index].m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, glData[shape_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* float_count, vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[shape_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_shapes[shape_index].mesh.indices.size() * sizeof(unsigned int), a_shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // Normal data

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*a_shapes[shape_index].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void ShadowsApp::LoadShaders()
{
	LoadShader((GLuint*)&shadowsProgramID, "../data/shaders/shadows_vertex.glsl", "../data/shaders/shadows_fragment.glsl", nullptr);
	LoadShader((GLuint*)&shadowMapProgramID, "../data/shaders/shadow_map_vertex.glsl", "../data/shaders/shadow_map_fragment.glsl", nullptr);
}

void ShadowsApp::ReloadShaders()
{
	printf("reloaded Shaders\n");
	glfwSetTime(0.0);
	currentGameTime = 0.0f;
	glDeleteProgram(shadowsProgramID);
	glDeleteProgram(shadowMapProgramID);
	LoadShaders();
}