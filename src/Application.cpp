#include "Application.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

Application::Application()
{

}


Application::~Application()
{
	
}
void Application::setDefaults()
{
	this->AppName = "Application Default";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool Application::startup()
{
	
	if (!glfwInit())
	{
		return false;
	}
	printf("glfwInit Successful\n");
	this->window = glfwCreateWindow(this->ScreenSize.Width, this->ScreenSize.Height, this->AppName, nullptr, nullptr);
	if (this->window == nullptr)
	{
		return false;
	}
	printf("glfwCreateWindow Successful\n");
	glfwMakeContextCurrent(this->window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(this->window);
		glfwTerminate();
		return false;
	}
	int Major_Version = ogl_GetMajorVersion();
	int Minor_Version = ogl_GetMinorVersion();

	printf("Opengl Version: %d.%d Loaded\n", Major_Version, Minor_Version);

	printf("************\n%s Init Complete\n", this->AppName);
	return true;
}

void Application::shutdown()
{
	if (this->window != nullptr)
	{
		glfwDestroyWindow(this->window);
	}
	glfwTerminate();
}

bool Application::update()
{
	if (glfwWindowShouldClose(this->window) )
	{
		return false;
	}
	return true;
}

void Application::draw()
{
	
}

GLFWwindow* Application::getWindow()
{
	return this->window;
}
