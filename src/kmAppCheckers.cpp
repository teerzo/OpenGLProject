#include "kmAppCheckers.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

kmAppCheckers::~kmAppCheckers() {

}

void kmAppCheckers::SetApplicationDefaults() {
	this->applicationName = "kmAppCheckers";
	this->screen_size.width = 1280;
	this->screen_size.height = 720;
}

bool kmAppCheckers::ApplicationStartup() {
	if (!Application::ApplicationStartup()) {
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable( GL_CULL_FACE );


	deferred_render = new RenderTarget(screen_size);

	// Load Shadow Frame Buffer
	
	pointLight = GenerateCube();
	
	tweak_light_direction = glm::vec3(1, -1, 0);
	tweak_light_color = glm::vec3(1, 1, 1);
	tweak_light_intensity = 1.0f;
	tweakBarLighting = TwNewBar("Lighting");
	TwAddVarRW(tweakBarLighting, "Direction", TW_TYPE_DIR3F, &tweak_light_direction, "group=Light1");
	TwAddVarRW(tweakBarLighting, "Color", TW_TYPE_COLOR3F, &tweak_light_color, "group=Light1");
	TwAddVarRW(tweakBarLighting, "Intensity", TW_TYPE_FLOAT, &tweak_light_intensity, "group=Light1");

	Reload();
	ReloadShaders();

	return true;
}

void kmAppCheckers::ApplicationShutdown() {
	Application::ApplicationShutdown();
}

bool kmAppCheckers::Update()
{
	CheckInput();
	if (!Application::Update()) {
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

void kmAppCheckers::Draw() {
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

	Application::Draw();
	DebugDraw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void kmAppCheckers::RenderGeometry() {
	glViewport(0, 0, deferred_render->screen.width, deferred_render->screen.height);
	glBindFramebuffer(GL_FRAMEBUFFER, deferred_render->gbuffer_fbo);
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(gBufferProgramID);

	unsigned int	uniform_proj_view = glGetUniformLocation(gBufferProgramID, "projection_view");
	unsigned int	uniform_view = glGetUniformLocation(gBufferProgramID, "view");
	unsigned int	uniform_world = glGetUniformLocation(gBufferProgramID, "world");
	int uniform_time = glGetUniformLocation(gBufferProgramID, "time");
	int uniform_delta_time = glGetUniformLocation(gBufferProgramID, "delta_time");

	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetView());
	// Set world on object
	glUniform1f(uniform_time, time.current);
	glUniform1f(uniform_delta_time, time.delta_time);
	
	if (mode.debug){ DebugDraw(); }	
}

void kmAppCheckers::RenderLights() {
	glBindFramebuffer(GL_FRAMEBUFFER, deferred_render->light_buffer_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawDirectionLight(tweak_light_direction, tweak_light_color);
	//DrawDirectionLight(cameraVector[currentCamera]->GetForward(), tweak_light_color);

	//DrawPointLight(cameraVector[0]->GetPosition(), 100, color.White.xyz);

}
void kmAppCheckers::DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color) {
	glUseProgram(pointLightProgramID);
	glm::vec4 view_space_pos = cameraVector[currentCamera]->GetView() * glm::vec4(a_position, 1);

	int uniform_pos = glGetUniformLocation(pointLightProgramID, "light_position");
	int uniform_view_pos = glGetUniformLocation(pointLightProgramID, "light_view_position");
	int uniform_light_color = glGetUniformLocation(pointLightProgramID, "light_color");
	int uniform_light_radius = glGetUniformLocation(pointLightProgramID, "light_radius");
	int uniform_proj_view = glGetUniformLocation(pointLightProgramID, "projection_view");

	glUniform3fv(uniform_pos, 1, (float*)&a_position);
	glUniform3fv(uniform_view_pos, 1, (float*)&view_space_pos);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);
	glUniform1f(uniform_light_radius, a_radius);
	glUniformMatrix4fv(uniform_proj_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	unsigned int uniform_position_texture = glGetUniformLocation(pointLightProgramID, "position_texture");
	glUniform1i(uniform_position_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, deferred_render->texture_position);
	unsigned int uniform_normals_texture = glGetUniformLocation(pointLightProgramID, "normal_texture");
	glUniform1i(uniform_normals_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deferred_render->texture_normal);

	glBindVertexArray(pointLight.m_VAO);
	glDrawElements(GL_TRIANGLES, pointLight.m_index_count, GL_UNSIGNED_INT, 0);
}

void kmAppCheckers::DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color) {
	glUseProgram(directionalLightProgramID);

	glm::vec4 view_space_light = cameraVector[currentCamera]->GetView() * glm::vec4(glm::normalize(a_direction), 0);

	int uniform_light_direction = glGetUniformLocation(directionalLightProgramID, "light_dir");
	int uniform_light_color = glGetUniformLocation(directionalLightProgramID, "light_color");
	unsigned int	uniform_view = glGetUniformLocation(directionalLightProgramID, "view");

	glUniform3fv(uniform_light_direction, 1, (float*)&view_space_light);
	glUniform3fv(uniform_light_color, 1, (float*)&a_color);
	glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetView());

	unsigned int uniform_position_texture = glGetUniformLocation(directionalLightProgramID, "position_texture");
	glUniform1i(uniform_position_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, deferred_render->texture_position);

	unsigned int uniform_normals_texture = glGetUniformLocation(directionalLightProgramID, "normals_texture");
	glUniform1i(uniform_normals_texture, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deferred_render->texture_normal);


	glBindVertexArray( deferred_render->m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, deferred_render->m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void kmAppCheckers::RenderComposite() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(compositeProgramID);

	glUniform1i(glGetUniformLocation(compositeProgramID, "albedo_texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, deferred_render->texture_albedo);

	glUniform1i(glGetUniformLocation(compositeProgramID, "light_texture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deferred_render->light_buffer_texture);

	glUniformMatrix4fv(glGetUniformLocation(compositeProgramID, "projection_view"), 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	glBindVertexArray(deferred_render->m_plane.m_VAO);
	glDrawElements(GL_TRIANGLES, deferred_render->m_plane.m_index_count, GL_UNSIGNED_INT, 0);
}

void kmAppCheckers::DebugDraw() {
	Application::DebugDraw();
}

void kmAppCheckers::CheckInput() {
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		ReloadShaders();
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		Reload();
	}
	Application::CheckInput();
}



void kmAppCheckers::LoadShaders() {
	LoadShader((GLuint*)&gBufferProgramID, "ass1_gbuffer_vertex.glsl", "ass1_gbuffer_fragment.glsl", nullptr);
	LoadShader((GLuint*)&compositeProgramID, "ass1_composite_vertex.glsl", "ass1_composite_fragment.glsl", nullptr);

	LoadShader((GLuint*)&directionalLightProgramID, "ass1_directional_light_vertex.glsl", "ass1_directional_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&pointLightProgramID, "ass1_point_light_vertex.glsl", "ass1_point_light_fragment.glsl", nullptr);
	LoadShader((GLuint*)&spotLightProgramID, "shadow_map_vertex.glsl", "shadow_map_fragment.glsl", nullptr);

}

void kmAppCheckers::Reload()
{
	glfwSetTime( 0.0 );
	time.current = 0.0f;
	time.previous = 0.0f;
	time.delta_time = 0.0f;
}

void kmAppCheckers::ReloadShaders() {
	printf("reloaded Shaders\n");
	
	
	glDeleteProgram(gBufferProgramID);
	glDeleteProgram(directionalLightProgramID);
	glDeleteProgram(pointLightProgramID);
	glDeleteProgram(spotLightProgramID);
	glDeleteProgram(compositeProgramID);
	LoadShaders();
}


