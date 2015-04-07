#include "TemplateApplication.h"

//#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

TemplateApplication::~TemplateApplication()
{

}

void TemplateApplication::SetApplicationDefaults()
{
	this->applicationName = "TemplateApplication";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool TemplateApplication::ApplicationStartup()
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
	
	


	return true;
}

void TemplateApplication::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool TemplateApplication::Update()
{
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

void TemplateApplication::Draw()
{
	glClearColor(defaultBackgroundColour.x, defaultBackgroundColour.y, defaultBackgroundColour.z, defaultBackgroundColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	}
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

void TemplateApplication::DebugDraw()
{
	Application::DebugDraw();
}

void TemplateApplication::CheckInput()
{
	
}