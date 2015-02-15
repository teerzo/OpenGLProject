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
	float currentTime = (float)glfwGetTime();
	float deltaTime = currentTime - previousTime; // prev of last frame
	previousTime = currentTime;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::clear();

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, -10), glm::vec3(-10 + i, 0, 10), i == 10 ? color.White : color.Gray);
		Gizmos::addLine(glm::vec3(-10, 0, -10 + i), glm::vec3(10, 0, -10 + i), i == 10 ? color.White : color.Gray);
	}
	
	for (unsigned i = 0; i < m_vListofCameras.size(); ++i)
	{
		m_vListofCameras[i]->update(deltaTime);
		Gizmos::addAABB(m_vListofCameras[i]->getPosition(), glm::vec3(1.2,1.2,1.2), color.Red);
		Gizmos::addLine(m_vListofCameras[i]->getPosition(), m_vListofCameras[i]->getPosition() + ( m_vListofCameras[i]->getForward() * -2 ), color.Blue);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_vListofCameras[0]->m_bIsSelected = true;
		m_vListofCameras[1]->m_bIsSelected = false;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_vListofCameras[0]->m_bIsSelected = false;
		m_vListofCameras[1]->m_bIsSelected = true;
	}

	Gizmos::addAABB(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), color.Blue);

	Gizmos::draw(m_vListofCameras[0]->getProjectionView());

	return true;
}

void CameraAndProjections::draw()
{
	


	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
