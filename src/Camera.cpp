#include "Camera.h"
#include <glfw3.h>
#include "Gizmos.h"

#include "glm_header.h"


Camera::Camera()
{
	printf("$Camera Created\n");

	m_mViewTransform = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	m_mProjectionTransform = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	setPosition(glm::vec3(0));
	GetFrustrumPlanes();
}

void Camera::DestroyCamera()
{

}

void Camera::SetViewProjection(mat4 a_Matrix)
{
	m_mProjectionViewTransform = a_Matrix;
}

void Camera::updateProjectionViewTransform()
{
	m_mProjectionViewTransform = m_mProjectionTransform * m_mViewTransform;
}

void Camera::update(float a_deltaTime)
{
	m_mViewTransform = glm::inverse(m_mWorldTransform);
	updateProjectionViewTransform();
}
void Camera::setPerspective(float a_FOV, float a_aspectRatio, float a_near, float a_far)
{
	m_mProjectionTransform = glm::perspective(glm::radians(a_FOV), a_aspectRatio, a_near, a_far);
	updateProjectionViewTransform();
}
void Camera::setLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up)
{
	//
	m_mViewTransform = glm::lookAt(a_from, a_to, a_up);
	m_mWorldTransform = glm::inverse(m_mViewTransform);
}
void Camera::setPosition(glm::vec3 a_position)
{
	//
	m_mWorldTransform = glm::translate(a_position);
	m_mViewTransform = glm::inverse(m_mWorldTransform);
}
glm::mat4 Camera::getWorldTransform()
{
	glm::mat4 temp = m_mWorldTransform;
	temp[1] *= -1;
	//temp[2] *= -1;
	temp[3].xyz = getPosition();
	return temp;
}
glm::mat4 Camera::getViewTransform()
{
	return m_mViewTransform;
}
glm::mat4 Camera::getProjection()
{
	return m_mProjectionTransform;
}
glm::mat4 Camera::getProjectionView()
{
	return m_mProjectionViewTransform;
}
glm::vec3 Camera::getPosition()
{
	return m_mWorldTransform[3].xyz;
}
glm::vec3 Camera::getUp()
{
	glm::vec3 temp;
	temp = m_mWorldTransform[1].xyz;
	//temp *= -1;
	return temp;
}

glm::vec3 Camera::getForward()
{
	glm::vec3 temp;
	temp = m_mWorldTransform[2].xyz;
	temp *= -1;
	return temp;
}

glm::vec3 Camera::getRight()
{
	glm::vec3 temp;
	temp = m_mWorldTransform[0].xyz;
	temp.x *= 1;
	return temp;
}

void Camera::GetFrustrumPlanes()
{
	// right side
	m_FrustrumPlanes[0] = vec4(m_mProjectionViewTransform[0][3] - m_mProjectionViewTransform[1][0],
		m_mProjectionViewTransform[1][3] - m_mProjectionViewTransform[1][0],
		m_mProjectionViewTransform[2][3] - m_mProjectionViewTransform[2][0],
		m_mProjectionViewTransform[3][3] - m_mProjectionViewTransform[3][0]);
	// left side
	m_FrustrumPlanes[1] = vec4(m_mProjectionViewTransform[0][3] + m_mProjectionViewTransform[0][0],
		m_mProjectionViewTransform[1][3] + m_mProjectionViewTransform[1][0],
		m_mProjectionViewTransform[2][3] + m_mProjectionViewTransform[2][0],
		m_mProjectionViewTransform[3][3] + m_mProjectionViewTransform[3][0]);
	// top
	m_FrustrumPlanes[2] = vec4(m_mProjectionViewTransform[0][3] - m_mProjectionViewTransform[0][1],
		m_mProjectionViewTransform[1][3] - m_mProjectionViewTransform[1][1],
		m_mProjectionViewTransform[2][3] - m_mProjectionViewTransform[2][1],
		m_mProjectionViewTransform[3][3] - m_mProjectionViewTransform[3][1]);
	// bottom
	m_FrustrumPlanes[3] = vec4(m_mProjectionViewTransform[0][3] + m_mProjectionViewTransform[0][1],
		m_mProjectionViewTransform[1][3] + m_mProjectionViewTransform[1][1],
		m_mProjectionViewTransform[2][3] + m_mProjectionViewTransform[2][1],
		m_mProjectionViewTransform[3][3] + m_mProjectionViewTransform[3][1]);
	// far
	m_FrustrumPlanes[4] = vec4(m_mProjectionViewTransform[0][3] - m_mProjectionViewTransform[0][2],
		m_mProjectionViewTransform[1][3] - m_mProjectionViewTransform[1][2],
		m_mProjectionViewTransform[2][3] - m_mProjectionViewTransform[2][2],
		m_mProjectionViewTransform[3][3] - m_mProjectionViewTransform[3][2]);
	// near
	m_FrustrumPlanes[5] = vec4(m_mProjectionViewTransform[0][3] + m_mProjectionViewTransform[0][2],
		m_mProjectionViewTransform[1][3] + m_mProjectionViewTransform[1][2],
		m_mProjectionViewTransform[2][3] + m_mProjectionViewTransform[2][2],
		m_mProjectionViewTransform[3][3] + m_mProjectionViewTransform[3][2]);
	for (int i = 0; i < 6; i++)
		m_FrustrumPlanes[i] = glm::normalize(m_FrustrumPlanes[i]);
}

// FLY Camera
FlyCamera::FlyCamera(unsigned int a_CameraID)
{
	m_fSpeed = 2.0f;
	m_CameraID = a_CameraID;
}

void FlyCamera::update(float a_deltaTime)
{
	Camera::update(a_deltaTime);
	GetFrustrumPlanes(); 

	if (!m_bIsSelected)
	{
		glm::mat4 temp = this->getWorldTransform();
		temp *= -1;
		temp[3].xyz = this->getPosition();
		Gizmos::addTransform(temp, 1.0f);
		Gizmos::addSphere(this->getPosition(), 1.0f, 8, 8, color.Clear);
		Gizmos::addLine(this->getPosition(), this->getPosition() + this->getForward() * 3, color.Blue);
		Gizmos::addLine(this->getPosition() + m_FrustrumPlanes[0].xyz, this->getPosition() + (m_FrustrumPlanes[0].xyz * 5.0f), color.Yellow);
		Gizmos::addLine(this->getPosition() + m_FrustrumPlanes[1].xyz, this->getPosition() + (m_FrustrumPlanes[1].xyz * 5.0f), color.Yellow);
		Gizmos::addLine(this->getPosition() + m_FrustrumPlanes[2].xyz, this->getPosition() + (m_FrustrumPlanes[2].xyz * 5.0f), color.Yellow);
		Gizmos::addLine(this->getPosition() + m_FrustrumPlanes[3].xyz, this->getPosition() + (m_FrustrumPlanes[3].xyz * 5.0f), color.Yellow);
		Gizmos::addLine(this->getPosition() + m_FrustrumPlanes[4].xyz, this->getPosition() + (m_FrustrumPlanes[4].xyz * 5.0f), color.Yellow);
		Gizmos::addLine(this->getPosition() + m_FrustrumPlanes[5].xyz, this->getPosition() + (m_FrustrumPlanes[5].xyz * 5.0f), color.Yellow);
	}


	//Gizmos::addAABB(this->getPosition(), glm::vec3(1.2, 1.2, 1.2), color.Red);

	//Gizmos::addLine(getPosition(), getPosition() + (getForward() * -2), color.Blue);

	CheckKeys(a_deltaTime);
}
void FlyCamera::setSpeed(float a_speed)
{
	m_fSpeed = a_speed;
}
float FlyCamera::getSpeed()
{
	return m_fSpeed;
}

void FlyCamera::CheckKeys(float a_deltaTime)
{
	if (m_bIsSelected)
	{
		// Check w,a,s,d and mouse
		// move camera
		GLFWwindow* window = glfwGetCurrentContext();

		double x_delta, y_delta;
		float width = 1280.0f;
		float height = 720.0f;

		glfwGetCursorPos(window, &x_delta, &y_delta);

		x_delta -= (width / 2);
		y_delta -= (height / 2);

		x_delta /= (width / 2);
		y_delta /= (height / 2);

		x_delta *= -100 * a_deltaTime;
		y_delta *= -100 * a_deltaTime;


		//x_delta -= ()

		if (glfwGetMouseButton(window, 1))
		{
			glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
			glm::vec3 camera_right = (glm::vec3)m_mWorldTransform[0];

			glm::mat4 yaw = glm::rotate((float)x_delta, glm::vec3(0, 1, 0));
			glm::mat4 pitch = glm::rotate((float)y_delta, camera_right);
			glm::mat4 rot = yaw * pitch;

			m_mWorldTransform[0] = rot * m_mWorldTransform[0];
			m_mWorldTransform[1] = rot * m_mWorldTransform[1];
			m_mWorldTransform[2] = rot * m_mWorldTransform[2];
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			// Reset
			m_mViewTransform = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			// move forward
			m_mWorldTransform[3] -= m_mWorldTransform[2] * m_fSpeed * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			// move forward
			m_mWorldTransform[3] += m_mWorldTransform[2] * m_fSpeed * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			// move forward
			m_mWorldTransform[3] -= m_mWorldTransform[0] * m_fSpeed * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			// move forward
			m_mWorldTransform[3] += m_mWorldTransform[0] * m_fSpeed * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			// move forward
			m_mWorldTransform[3] -= m_mWorldTransform[1] * m_fSpeed * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			// move forward
			m_mWorldTransform[3] += m_mWorldTransform[1] * m_fSpeed * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			m_mWorldTransform[3].y += m_fSpeed * a_deltaTime;
		}
	}
	else
	{

	}
}

void FlyCamera::_DebugCurrentPos()
{
	//printf 
}

void RenderTargetCamera::update(float a_deltaTime)
{
	if (m_Target != nullptr)
	{
		// move to be inverse of target Camera
		vec3 position;
		position = m_Target->getPosition() + m_Target->getForward() * 3;

		this->setPosition(position);
	}
	/*glm::mat4 temp = this->getWorldTransform();
	temp *= -1;
	temp[3].xyz = this->getPosition();
	Gizmos::addTransform(temp, 1.0f);
	Gizmos::addSphere(this->getPosition(), 1.0f, 8, 8, color.Clear);*/


}
void RenderTargetCamera::SetTargetCamera(Camera* a_Camera)
{
	if (a_Camera != nullptr)
		m_Target = a_Camera;
}