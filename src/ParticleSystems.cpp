#include "ParticleSystems.h"
#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"
#include "Utility.h"

ParticleSystems::~ParticleSystems()
{

}

void ParticleSystems::setDefaults()
{
	this->AppName = "ParticleSystems";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool ParticleSystems::startup()
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
	//LoadShader("../data/shaders/particle_vertex.glsl", "../data/shaders/particle_fragment.glsl", (GLuint*)&m_ProgramID);
	
	LoadShader("particle", (GLuint*)&m_ProgramID);
	
	
	m_emitter.Initialise(emitter_type::direction , 100000, glm::vec4(0, 0, 0, 1), 100, 20, 30, 10, 15, 0.5f, 0.8f, color.Green, color.GreenClear);
	m_emitter.SetDirection(vec4(0, 1, 0, 0));

	return true;
}

void ParticleSystems::shutdown()
{
	Application::shutdown();
}

bool ParticleSystems::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////
	m_emitter.Update(m_fdeltaTime);

	m_emitter.UpdateVertexData(m_vListofCameras[ActiveCamera]->getPosition(), m_vListofCameras[ActiveCamera]->getUp());

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void ParticleSystems::draw()
{
	glClearColor(m_BackGroundColor.x, m_BackGroundColor.y, m_BackGroundColor.z, m_BackGroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_Basic_Program);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());

	}
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////
	glUseProgram(m_ProgramID);
	int basic_proj_view_handle = glGetUniformLocation(m_Basic_Program, "ProjectionView");
	if (basic_proj_view_handle >= 0)
	{
		glUniformMatrix4fv(basic_proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());

	}
	m_emitter.Render();

	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
