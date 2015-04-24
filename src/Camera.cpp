#include "Camera.h"
//#include "Utility.h"
#include <glfw3.h>
#include "Gizmos.h"
#include "glm_header.h"
#include "Utility.h"

Camera::Camera(const Screen screenres)
{
	speed_movement = 10.0f;
	speed_rotation = 2.0f;

	matrix_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_projection = glm::perspective(glm::radians(70.0f), (float)(screenres.width / screenres.height), 0.10f, 1000.0f);
}

void Camera::SetViewProjection(const glm::mat4 matrix)
{
	matrix_projection_view = matrix;
}

void Camera::Update(const float a_deltaTime)
{
	if (is_active)
	{
		matrix_view = glm::inverse(matrix_world);
		matrix_projection_view = matrix_projection * matrix_view;
	}
}
void Camera::SetPerspective(const float a_FOV, const float a_aspectRatio, const float a_near, const float a_far)
{
	matrix_projection = glm::perspective(glm::radians(a_FOV), a_aspectRatio, a_near, a_far);
}
void Camera::SetLookAt(const glm::vec3 a_from, const glm::vec3 a_to, const glm::vec3 a_up)
{
	matrix_view = glm::lookAt(a_from, a_to, a_up);
	matrix_world = glm::inverse(matrix_view);
}

glm::mat4 Camera::GetView()				{ return matrix_view; }
glm::mat4 Camera::GetProjection()		{ return matrix_projection; }
glm::mat4 Camera::GetProjectionView()	{ return matrix_projection_view; }





void Camera::CheckInput(const float a_deltaTime)
{
	if (is_selected)
	{
		GLFWwindow* window = glfwGetCurrentContext();
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		double x_delta, y_delta;
		glfwGetCursorPos(window, &x_delta, &y_delta);
		x_delta -= (width / 2);
		y_delta -= (height / 2);
		x_delta /= (width / 2);
		y_delta /= (height / 2);
		x_delta *= -100 * a_deltaTime;
		y_delta *= -100 * a_deltaTime;

		if (glfwGetMouseButton(window, 1))
		{
			glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
			glm::vec3 camera_right = (glm::vec3)matrix_world[0];

			glm::mat4 yaw = glm::rotate((float)x_delta, glm::vec3(0, 1, 0));
			glm::mat4 pitch = glm::rotate((float)y_delta, camera_right);
			glm::mat4 rot = yaw * pitch;

			matrix_world[0] = rot * matrix_world[0];
			matrix_world[1] = rot * matrix_world[1];
			matrix_world[2] = rot * matrix_world[2];
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			matrix_world[3] -= matrix_world[2] * speed_movement * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			matrix_world[3] += matrix_world[2] * speed_movement * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			matrix_world[3] -= matrix_world[0] * speed_movement * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			matrix_world[3] += matrix_world[0] * speed_movement * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			matrix_world[3] -= matrix_world[1] * speed_movement * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			matrix_world[3] += matrix_world[1] * speed_movement * a_deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			matrix_world[3].y += speed_movement * a_deltaTime;
		}
	}
}

