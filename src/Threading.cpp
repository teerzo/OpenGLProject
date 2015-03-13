#include "Threading.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

Threading::~Threading()
{

}


void Threading::setDefaults()
{
	this->AppName = "Threading";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool Threading::startup()
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

	

	//myThread.join();

	return true;
}

void Threading::shutdown()
{
	Application::shutdown();
}

bool Threading::update()
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

void Threading::draw()
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
