#include "Quaternions.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"

Quaternions::~Quaternions()
{

}

void Quaternions::setDefaults()
{
	this->AppName = "Quaternions";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
}

bool Quaternions::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	glfwSetTime(0.0);

	// Quaternions
	glm::quat my_quaternion(1, 0, 0, 0);
	glm::quat euler_quaternion(glm::vec3(3, 5, 7));

	glm::quat mixed_quat = glm::slerp(my_quaternion, euler_quaternion, 0.8f);



	float PI = 3.14159;

	m_hip_frames[0].position = glm::vec3(0, 5, 0);
	m_hip_frames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_knee_frames[0].position = glm::vec3(0, -2.5, 0);
	m_knee_frames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_ankle_frames[0].position = glm::vec3(0, -2.5, 0);
	m_ankle_frames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_foot_frames[0].position = glm::vec3(0, 0, -2);
	m_foot_frames[0].rotation = glm::quat( glm::vec3(-1, 0, 0));


	m_hip_frames[1].position = glm::vec3(0, 5, 0);
	m_hip_frames[1].rotation = glm::quat(glm::vec3(1, 0, 0));

	m_knee_frames[1].position = glm::vec3(0, -2.5, 0);
	m_knee_frames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_ankle_frames[1].position = glm::vec3(0, -2.5, 0);
	m_ankle_frames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_foot_frames[1].position = glm::vec3(0, 0, -2);
	m_foot_frames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	return true;
}

glm::mat4 EvaluateKeyFrames(KeyFrame start, KeyFrame end, float time)
{
	glm::vec3 pos = glm::mix(start.position, end.position, time);
	glm::quat rot = glm::slerp(start.rotation, end.rotation, time);
	glm::mat4 result = glm::translate(pos) * glm::toMat4(rot);
	return result;
}

void Quaternions::shutdown()
{
	Application::shutdown();
}

bool Quaternions::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////
	// sin wave from -1 to 1 into 0 to 1 range
	float sin_wave = sinf(m_fTimer*5) * 0.5f + 0.5f;

	float temp = sin_wave;

	m_hip_bone = EvaluateKeyFrames(m_hip_frames[0], m_hip_frames[1], temp);
	m_knee_bone = EvaluateKeyFrames(m_knee_frames[0], m_knee_frames[1], temp);
	m_ankle_bone = EvaluateKeyFrames(m_ankle_frames[0], m_ankle_frames[1], temp);
	m_foot_bone = EvaluateKeyFrames(m_foot_frames[0], m_foot_frames[1], temp);

	glm::mat4 global_hip = m_hip_bone;
	glm::mat4 global_knee = global_hip * m_knee_bone;
	glm::mat4 global_ankle = global_knee * m_ankle_bone;
	glm::mat4 global_foot = global_ankle * m_foot_bone;

	glm::vec3 hip_pos = global_hip[3].xyz;
	glm::vec3 knee_pos = global_knee[3].xyz;
	glm::vec3 ankle_pos = global_ankle[3].xyz;
	glm::vec3 foot_pos = global_foot[3].xyz;

	Gizmos::addAABBFilled(hip_pos, glm::vec3(0.5f), color.Red, &global_hip);
	Gizmos::addAABBFilled(knee_pos, glm::vec3(0.5f), color.Green, &global_knee);
	Gizmos::addAABBFilled(ankle_pos, glm::vec3(0.5f), color.Blue, &global_ankle);
	Gizmos::addAABBFilled(foot_pos, glm::vec3(0.5f), color.Yellow, &global_foot);

	Gizmos::addLine(hip_pos, knee_pos, color.Red, color.Green);
	Gizmos::addLine(knee_pos, ankle_pos, color.Green, color.Blue);
	Gizmos::addLine(ankle_pos, foot_pos, color.Blue, color.Yellow);

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void Quaternions::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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
