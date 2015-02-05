#include "gl_core_4_4.h"
#include <glfw3.h>
#include "Gizmos.h"
#include <cstdio>

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

int main(int argc, char *argv[])
{
	
	if (!glfwInit())
	{
		return -1;
	}
	printf( "glfwInit Successful\n" );
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Graphics", nullptr, nullptr);
	if (window == nullptr)
	{
		return -2;
	}
	printf("glfwCreateWindow Successful\n");
	glfwMakeContextCurrent(window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	int Major_Version = ogl_GetMajorVersion();
	int Minor_Version = ogl_GetMinorVersion();

	printf("Opengl Version: %d.%d Loaded\n", Major_Version, Minor_Version);
	
	printf("************\nOpengl Init Complete\n");

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	float rotate_x1 = 10.0f;
	float rotate_x2 = 10.0f;
	float rotate_z1 = 10.0f;
	float rotate_z2 = 10.0f;
	float timer = 0.0f;
	vec4 White(1);
	vec4 Red(1.0, 0, 0, 1);
	vec4 Yellow(0.5, 0.5, 0, 1);
	vec4 Gray(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 Black(0, 0, 0, 1);
	
	
	mat4 ProjectionMatrix = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	while (glfwWindowShouldClose(window) == false )
	{
		// Cool code here please
		float dt = (float)glfwGetTime();
		glfwSetTime(0.0);
		timer += dt;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Gizmos::clear();

		Gizmos::addTransform(mat4(1));
	
		rotate_x1 = sinf(timer) * 5;
		rotate_x2 = sinf(timer) * 50;
		rotate_z1 = cosf(timer) * 5;
		rotate_z2 = cosf(timer) * 50;
		mat4 SunTransform = glm::rotate(timer, vec3(0, 1, 0));
		mat4 Child1 = SunTransform * glm::translate(vec3(5,0,0 )) * glm::rotate(timer * 2.0f, vec3(0, 1, 0));
		mat4 Disk1 = rotate(timer* 10.0f, vec3(0, 0, 1)) * Child1;
		mat4 Child2 = Child1 * glm::rotate(timer * 10.0f, vec3(0, 1, 0)) * glm::translate(vec3(2, 0, 0));
		
		mat4 ViewMatrix = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
		
		for(int i = 0; i <= 20; ++i)
		{
			Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10), i == 10 ? White : Gray);
			Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i), i == 10 ? White : Gray);
		}
		
		Gizmos::addSphere(SunTransform[3].xyz, 2.0f, 16, 16, Yellow, &SunTransform);
		Gizmos::addSphere(Child1[3].xyz, 1.0f, 16, 16, Red, &Child1);
		Gizmos::addSphere(Child2[3].xyz, 0.5f, 16, 16, White, &Child2);
		Gizmos::addDisk(Child1[3].xyz, 2.0f, 16, Gray, &Child1);

		Gizmos::draw(ProjectionMatrix, ViewMatrix);
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	if (window != nullptr)
	{
		glfwDestroyWindow(window);
	}
	glfwTerminate();
	return 0;
}