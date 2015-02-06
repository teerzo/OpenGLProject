#include "CameraAndProjections.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>


#include "Gizmos.h"

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

	
	ProjectionMatrix = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);


	glfwSetTime(0.0);

	return true;
}

void CameraAndProjections::shutdown()
{
	Gizmos::destroy();
}

bool CameraAndProjections::update()
{
	if (!Application::update())
	{
		return false;
	}
	// Cool code here please
	float dt = (float)glfwGetTime();
	timer += dt;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::clear();

	

	return true;
}

void CameraAndProjections::draw()
{
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
