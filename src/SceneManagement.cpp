#include "SceneManagement.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

SceneManagement::~SceneManagement()
{

}

void SceneManagement::setDefaults()
{
	this->AppName = "SceneManagement";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool SceneManagement::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Gizmos::create();
	glfwSetTime(0.0);


	plane = glm::vec4(0, 1, 0, -1);

	return true;
}

void SceneManagement::shutdown()
{
	Application::shutdown();
}

bool SceneManagement::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	// Collision Check
	BoundingSphere sphere;
	sphere.centre = vec3(0, cosf((float)glfwGetTime()) + 1, 0);
	sphere.radius = 0.5f;
	vec4 plane(0, 1, 0, -1);
	float d = glm::dot(vec3(plane), sphere.centre) + plane.w;
	Gizmos::addSphere(sphere.centre, sphere.radius,
		8, 8, color.Blue);
	vec4 planeColour = color.White;
	if (d > sphere.radius)
		planeColour = color.Green;
	else if (d < -sphere.radius)
		planeColour = color.Red;
	Gizmos::addTri(vec3(4, 1, 4), vec3(-4, 1, -4), vec3(-4, 1, 4),
		planeColour);
	Gizmos::addTri(vec3(4, 1, 4), vec3(4, 1, -4), vec3(-4, 1, -4),
		planeColour);

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void SceneManagement::draw()
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
