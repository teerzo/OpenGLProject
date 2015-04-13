#include "DeferredRendering.h"

//#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

DeferredRendering::~DeferredRendering()
{

}

void DeferredRendering::SetApplicationDefaults()
{
	this->applicationName = "DeferredRendering";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool DeferredRendering::ApplicationStartup()
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

	glEnable(GL_CULL_FACE);

	// Load Shadow Frame Buffer
	gBuffer.SetWindowSize(1280, 720);
	gBuffer.SetPlaneSize(1280, 720);
	gBuffer.GenerateTarget();

	gBuffer.GenerateGBuffer();
	gBuffer.GenerateLightBuffer();

	pointLight = GenerateCube();

	// Load Plane
	//glPlane = GeneratePlane();
	// Load Mesh
	

	//createOpenGLBuffers(m_fbx_file);
	
	bunnyObject = new MeshGroup();

	if (!LoadOBJFile(bunnyObject, "stanford/bunny.obj")) {
		return false;
	}
	//CreateOpenGLBuffers(shapes);
	// Load Shaders
	LoadShaders();


	return true;
}

void DeferredRendering::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool DeferredRendering::Update()
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

void DeferredRendering::Draw()
{
	//glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST); 
	RenderGeometry();
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	RenderLights();
	glDisable(GL_BLEND);
	
	RenderComposite();
		
	//////////////////////
	//! End of Draw Code 
	//////////////////////
	DebugDraw();
	Application::Draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void DeferredRendering::RenderGeometry()
{	
	glViewport(0, 0, gBuffer.m_screen.Width, gBuffer.m_screen.Height);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.m_fbo);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gBufferProgramID);

	unsigned int	uniform_view		= glGetUniformLocation(gBufferProgramID, "View");
	unsigned int	uniform_proj_view	= glGetUniformLocation(gBufferProgramID, "ProjectionView");
	glUniformMatrix4fv(uniform_view,	  1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetView());
	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	bunnyObject->Draw();


	//glCullFace(GL_BACK); 
	//glBindVertexArray(pointLight.m_VAO);
	//glDrawElements(GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendering::RenderLights()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.light_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawDirectionLight(cameraVector[0]->GetForward(), color.White.xyz);

	//DrawPointLight(cameraVector[0]->GetPosition(), 10, color.White.xyz);
	
}
void DeferredRendering::DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color)
{
	glUseProgram(pointLightProgramID);
	glm::vec4 view_space_pos = cameraVector[currentCamera]->GetView() * glm::vec4(a_position, 1);

	int uniform_pos = glGetUniformLocation(pointLightProgramID, "light_position");
	int uniform_view_pos = glGetUniformLocation(pointLightProgramID, "light_view_position");
	int uniform_light_color = glGetUniformLocation(pointLightProgramID, "light_color");
	int uniform_light_radius = glGetUniformLocation(pointLightProgramID, "light_radius");
	int uniform_proj_view = glGetUniformLocation(pointLightProgramID, "ProjectionView");

	glUniform3fv(uniform_pos, 1, (float*)&a_position);
	glUniform3fv(uniform_view_pos, 1, (float*)&view_space_pos);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);
	glUniform1f(uniform_light_radius, a_radius);
	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	unsigned int uniform_position_texture = glGetUniformLocation(pointLightProgramID, "position_texture");
	glUniform1i(uniform_position_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.position_texture);
	unsigned int uniform_normals_texture = glGetUniformLocation(pointLightProgramID, "normals_texture");
	glUniform1i(uniform_normals_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.normals_texture);

	glBindVertexArray(pointLight.m_VAO);
	glDrawElements(GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0);
}

void DeferredRendering::DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color)
{
	glUseProgram(directionalLightProgramID);

	glm::vec4 view_space_light = cameraVector[currentCamera]->GetView() * glm::vec4(glm::normalize(a_direction), 0);

	int uniform_light_direction = glGetUniformLocation(directionalLightProgramID, "light_dir");
	int uniform_light_color = glGetUniformLocation(directionalLightProgramID, "light_color");

	glUniform3fv(uniform_light_direction, 1, (float*)&view_space_light);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);

	unsigned int uniform_position_texture = glGetUniformLocation(directionalLightProgramID, "position_texture");
	glUniform1i(uniform_position_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.position_texture);
	unsigned int uniform_normals_texture = glGetUniformLocation(directionalLightProgramID, "normals_texture");
	glUniform1i(uniform_normals_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.normals_texture);

	glBindVertexArray(gBuffer.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void DeferredRendering::RenderComposite()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(compositeProgramID);

	glUniform1i(glGetUniformLocation(compositeProgramID, "albedo_texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.albedo_texture);

	glUniform1i(glGetUniformLocation(compositeProgramID, "light_texture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.light_texture);

	glUniformMatrix4fv(glGetUniformLocation(compositeProgramID, "ProjectionView"), 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	glBindVertexArray(gBuffer.m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, gBuffer.m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void DeferredRendering::DebugDraw()
{
	Application::DebugDraw();
}

void DeferredRendering::CheckInput()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShaders();
	}
	Application::CheckInput();
}



void DeferredRendering::LoadShaders()
{
	LoadShader((GLuint*)&gBufferProgramID, "gbuffer_vertex.glsl", "gbuffer_fragment.glsl", nullptr);
	LoadShader((GLuint*)&compositeProgramID, "composite_vertex.glsl", "composite_fragment.glsl", nullptr);

	LoadShader((GLuint*)&directionalLightProgramID, "directional_light_vertex.glsl", "directional_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&pointLightProgramID, "point_light_vertex.glsl", "point_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&spotLightProgramID, "shadow_map_vertex.glsl", "shadow_map_fragment.glsl", nullptr);
}

void DeferredRendering::ReloadShaders()
{
	printf("reloaded Shaders\n");
	glfwSetTime(0.0);
	currentGameTime = 0.0f;
	glDeleteProgram(gBufferProgramID);
	glDeleteProgram(directionalLightProgramID);
	glDeleteProgram(pointLightProgramID);
	glDeleteProgram(spotLightProgramID);
	glDeleteProgram(compositeProgramID);
	LoadShaders();
}


