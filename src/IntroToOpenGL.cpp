#include "IntroToOpenGL.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

IntroToOpenGL::~IntroToOpenGL()
{
		
}


void IntroToOpenGL::setDefaults()
{
	this->AppName = "Intro to OpenGL";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool IntroToOpenGL::startup()
{	
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	glfwSetTime(0.0);
	
	 rotate_x1 = 10.0f;
	 rotate_x2 = 10.0f;
	 rotate_z1 = 10.0f;
	 rotate_z2 = 10.0f;
	 	
	return true;
}

void IntroToOpenGL::shutdown()
{
	Application::shutdown();
}

bool IntroToOpenGL::update()
{
	if (!Application::update())
	{
		return false; 
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////
	
	
	// $$$ fix this!
	
	Gizmos::addTransform(mat4(1));

	rotate_x1 = sinf(m_fTimer) * 5;
	rotate_x2 = sinf(m_fTimer) * 50;
	rotate_z1 = cosf(m_fTimer) * 5;
	rotate_z2 = cosf(m_fTimer) * 50;
	mat4 SunTransform = glm::rotate(m_fTimer, vec3(0, 1, 0));
	mat4 Child1 = SunTransform * glm::translate(vec3(5, 0, 0)) * glm::rotate(m_fTimer * 2.0f, vec3(0, 1, 0));
	mat4 Disk1 = glm::rotate(m_fTimer* 10.0f, vec3(0, 0, 1)) * Child1;
	mat4 Child2 = Child1 * glm::rotate(m_fTimer * 10.0f, vec3(0, 1, 0)) * glm::translate(vec3(2, 0, 0));
		
	Gizmos::addSphere(SunTransform[3].xyz, 2.0f, 16, 16, color.Yellow, &SunTransform);
	Gizmos::addSphere(Child1[3].xyz, 1.0f, 16, 16, color.Red, &Child1);
	Gizmos::addSphere(Child2[3].xyz, 0.5f, 16, 16, color.White, &Child2);
	Gizmos::addDisk(Child1[3].xyz, 2.0f, 16, color.Gray, &Child1);


	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void IntroToOpenGL::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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
