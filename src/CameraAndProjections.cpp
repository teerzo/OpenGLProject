#include "CameraAndProjections.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

CameraAndProjections::~CameraAndProjections()
{

}

void CameraAndProjections::setDefaults()
{
	this->AppName = "Camera and Projections";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;	
}

bool CameraAndProjections::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	glfwSetTime(0.0);

	return true;
}

void CameraAndProjections::shutdown()
{
	Application::shutdown();
}

bool CameraAndProjections::update()
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

void CameraAndProjections::draw()
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
