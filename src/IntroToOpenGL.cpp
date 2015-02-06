#include "IntroToOpenGL.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"

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
	
	 rotate_x1 = 10.0f;
	 rotate_x2 = 10.0f;
	 rotate_z1 = 10.0f;
	 rotate_z2 = 10.0f;
	 timer = 0.0f;
	 White = vec4(1);
	 Red = vec4(1.0, 0, 0, 1);
	 Yellow = vec4(0.5, 0.5, 0, 1);
	 Gray = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	 Black = vec4(0, 0, 0, 1);
	ProjectionMatrix = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);


	glfwSetTime(0.0);
	
	return true;
}

void IntroToOpenGL::shutdown()
{
	Gizmos::destroy();
}

bool IntroToOpenGL::update()
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

	Gizmos::addTransform(mat4(1));

	rotate_x1 = sinf(timer) * 5;
	rotate_x2 = sinf(timer) * 50;
	rotate_z1 = cosf(timer) * 5;
	rotate_z2 = cosf(timer) * 50;
	mat4 SunTransform = glm::rotate(timer, vec3(0, 1, 0));
	mat4 Child1 = SunTransform * glm::translate(vec3(5, 0, 0)) * glm::rotate(timer * 2.0f, vec3(0, 1, 0));
	mat4 Disk1 = rotate(timer* 10.0f, vec3(0, 0, 1)) * Child1;
	mat4 Child2 = Child1 * glm::rotate(timer * 10.0f, vec3(0, 1, 0)) * glm::translate(vec3(2, 0, 0));

	mat4 ViewMatrix = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? White : Gray);
		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? White : Gray);
	}

	Gizmos::addSphere(SunTransform[3].xyz, 2.0f, 16, 16, Yellow, &SunTransform);
	Gizmos::addSphere(Child1[3].xyz, 1.0f, 16, 16, Red, &Child1);
	Gizmos::addSphere(Child2[3].xyz, 0.5f, 16, 16, White, &Child2);
	Gizmos::addDisk(Child1[3].xyz, 2.0f, 16, Gray, &Child1);

	Gizmos::draw(ProjectionMatrix, ViewMatrix);


	return true;
}

void IntroToOpenGL::draw()
{
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
