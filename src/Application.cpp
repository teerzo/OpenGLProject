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

	ActiveCamera = 0; 
	AddFlyCamera();

	mode.LockCamera = false;
	mode.Debug = true;
	mode.Wireframe = false;
	mode.HideObjects = false;

	m_fDelayTimer = 0.0f;
	m_fDelayMax = 1.0f;

	printf("************\n%s Init Complete\n", this->AppName);

	
	return true;
}

void Application::shutdown()
{
	if (this->window != nullptr)
	{
		glfwDestroyWindow(this->window);
	}
	Gizmos::destroy();
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

	if (m_fDelayTimer < m_fDelayMax)
	{
		m_fDelayTimer += m_fdeltaTime;
	}

	Gizmos::clear();

	_DrawGrid();

	_UpdateCameras();
	
	_CheckKeys();
	
	return true;
}

void Application::draw()
{
	// Base Draw Code
	Gizmos::draw(m_vListofCameras[ActiveCamera]->getProjectionView());
}

GLFWwindow* Application::getWindow()
{
	return this->window;
}

void Application::_DrawGrid()
{
	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, -10), glm::vec3(-10 + i, 0, 10), i == 10 ? color.White : color.Gray);
		Gizmos::addLine(glm::vec3(-10, 0, -10 + i), glm::vec3(10, 0, -10 + i), i == 10 ? color.White : color.Gray);
	}
}

void Application::_UpdateCameras()
{
	for (unsigned i = 0; i < m_vListofCameras.size(); ++i)
	{
		m_vListofCameras[i]->update(m_fdeltaTime);
		if (mode.Debug)
		{
			Gizmos::addAABB(m_vListofCameras[i]->getPosition(), glm::vec3(1.2, 1.2, 1.2), color.Red);
			Gizmos::addLine(m_vListofCameras[i]->getPosition(), m_vListofCameras[i]->getPosition() + (m_vListofCameras[i]->getForward() * -2), color.Blue);
		}
	}
}

void Application::_CheckKeys()
{
	// hotkeys.... move to own function
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			ActiveCamera = 0;
			m_fDelayTimer = 0.0f;
			_DisableOtherCameras();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		if (ActiveCamera - 1 >= 0)
		{
			if (m_fDelayTimer >= m_fDelayMax)
			{
				ActiveCamera -= 1;
				m_fDelayTimer = 0.0f;
				_DisableOtherCameras();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		if ((unsigned int)ActiveCamera + 1 < m_vListofCameras.size() )
		{
			if (m_fDelayTimer >= m_fDelayMax)
			{
				ActiveCamera += 1;
				m_fDelayTimer = 0.0f;
				_DisableOtherCameras();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			AddFlyCamera();
			m_fDelayTimer = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			DestroyActiveCamera();
			m_fDelayTimer = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		mode.Debug = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		mode.Debug = false;
	}
}

void Application::_DisableOtherCameras()
{
	if ((unsigned int)ActiveCamera < m_vListofCameras.size())
	{
		if (!mode.LockCamera)
		{
			m_vListofCameras[ActiveCamera]->m_bIsSelected = true;
			printf("~Camera : %d\n", ActiveCamera);
		}
		for (unsigned int i = 0; i < m_vListofCameras.size(); ++i)
		{
			if (i != ActiveCamera)
			{
				m_vListofCameras[i]->m_bIsSelected = false;
			}
		}
	} 
}

void Application::AddFlyCamera()
{
	
	this->m_vListofCameras.push_back(new FlyCamera(m_vListofCameras.size()));
	ActiveCamera = m_vListofCameras.size()-1;
	printf("~New Camera : %d\n", ActiveCamera);
	_DisableOtherCameras();
	
}

void Application::DestroyActiveCamera()
{
	if (this->m_vListofCameras.size() > 1)
	{
		// delete active camera
		//this->m_vListofCameras.
	}
}