#include "Camera.h"
#include <glfw3.h>

Camera::Camera()
{
	printf("$Camera Created\n");

	m_mViewTransform = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0));
	m_mProjectionTransform = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
	setPosition(glm::vec3(0));
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
}
void Camera::setLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up)
{
	m_mViewTransform = glm::lookAt(a_from, a_to, a_up);
	m_mWorldTransform = glm::inverse(m_mViewTransform);
}
void Camera::setPosition(glm::vec3 a_position)
{
	m_mWorldTransform = glm::translate(a_position);
	m_mViewTransform = glm::inverse(m_mWorldTransform);
}
glm::mat4 Camera::getWorldTransform()
{
	return m_mWorldTransform;
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

glm::vec3 Camera::getForward()
{
	glm::vec3 temp;
	temp = m_mWorldTransform[2].xyz;
	temp.z *= 1;
	return temp;
}
// FLY Camera

FlyCamera::FlyCamera()
{
	printf("$Fly");
	m_fSpeed = 2.0f;
}

void FlyCamera::update(float a_deltaTime)
{
	Camera::update( a_deltaTime );

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

		x_delta *= -10 * a_deltaTime;
		y_delta *= -10 * a_deltaTime;


		//x_delta -= ()

		if (glfwGetMouseButton(window, 1))
		{
			glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
			glm::vec3 camera_right = (glm::vec3)m_mWorldTransform[0];

			glm::mat4 yaw = glm::rotate((float)x_delta, glm::vec3(0, 1, 0));
			glm::mat4 pitch = glm::rotate((float)y_delta, camera_right );
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
		
	}
	else
	{
		//setPosition(glm::vec3(0, 0, 0));
	}

}
void FlyCamera::setSpeed(float a_speed)
{
	m_fSpeed = a_speed;
}
float FlyCamera::getSpeed()
{
	return m_fSpeed;
}

