#include "TemplateApplication.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>


#include "Gizmos.h"

TemplateApplication::~TemplateApplication()
{

}


void TemplateApplication::setDefaults()
{
	this->AppName = "TemplateApplication";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool TemplateApplication::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();


	//ProjectionMatrix = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	//ViewMatrix = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), vec3(0, 1, 0));

	glfwSetTime(0.0);

	return true;
}

void TemplateApplication::shutdown()
{
	Gizmos::destroy();
}

bool TemplateApplication::update()
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

void TemplateApplication::draw()
{
	

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
