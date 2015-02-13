#include "Application.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Gizmos.h"

#include "Camera.h"

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

	mode.Debug = false;
	mode.Wireframe = false;

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
	if (glfwWindowShouldClose(this->window))
	{
		return false;
	}

	m_fTimer = (float)glfwGetTime();
	m_fdeltaTime = m_fTimer - m_fPreviousTime; // prev of last frame
	m_fPreviousTime = m_fTimer;

	Gizmos::clear();

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, -10), glm::vec3(-10 + i, 0, 10), i == 10 ? color.White : color.Gray);
		Gizmos::addLine(glm::vec3(-10, 0, -10 + i), glm::vec3(10, 0, -10 + i), i == 10 ? color.White : color.Gray);
	}
	for (unsigned i = 0; i < m_vListofCameras.size(); ++i)
	{
		m_vListofCameras[i]->update(m_fdeltaTime);
		if (mode.Debug)
		{
			Gizmos::addAABB(m_vListofCameras[i]->getPosition(), glm::vec3(1.2, 1.2, 1.2), color.Red);
			Gizmos::addLine(m_vListofCameras[i]->getPosition(), m_vListofCameras[i]->getPosition() + (m_vListofCameras[i]->getForward() * -2), color.Blue);
		}
	}
	// hotkeys.... move to own function
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		ActiveCamera = 0;
		m_vListofCameras[0]->m_bIsSelected = true;
		m_vListofCameras[1]->m_bIsSelected = false;
		m_vListofCameras[2]->m_bIsSelected = false;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		ActiveCamera = 1;
		m_vListofCameras[0]->m_bIsSelected = false;
		m_vListofCameras[1]->m_bIsSelected = true;
		m_vListofCameras[2]->m_bIsSelected = false;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		ActiveCamera = 2;
		m_vListofCameras[0]->m_bIsSelected = false;
		m_vListofCameras[1]->m_bIsSelected = false;
		m_vListofCameras[2]->m_bIsSelected = true;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		m_vListofCameras[0]->m_bIsSelected = false;
		m_vListofCameras[1]->m_bIsSelected = false;
		m_vListofCameras[2]->m_bIsSelected = true;
	}

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		mode.Debug = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		mode.Debug = false;
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
