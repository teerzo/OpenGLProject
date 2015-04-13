#include "Camera.h"
//#include "Utility.h"
#include <glfw3.h>
#include "Gizmos.h"
#include "glm_header.h"

Camera::Camera()
{
	printf("+ Camera Created\n");
	viewTransform = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projectionTransform = glm::perspective(glm::radians(70.0f), 1280.0f/720.0f, 0.10f, 1000.0f);
}

void Camera::DestroyCamera()
{

}

void Camera::SetViewProjection(glm::mat4 a_Matrix)
{
	projectionViewTransform = a_Matrix;
}

void Camera::UpdateProjectionView()
{
	projectionViewTransform = projectionTransform * viewTransform;
}

void Camera::Update(float a_deltaTime)
{
	viewTransform = glm::inverse(worldTransform);
	UpdateProjectionView();
}
void Camera::SetPerspective(const float a_FOV, const float a_aspectRatio, const float a_near, const float a_far)
{
	projectionTransform = glm::perspective(glm::radians(a_FOV), a_aspectRatio, a_near, a_far);
	UpdateProjectionView();
}
void Camera::SetLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up)
{
	viewTransform = glm::lookAt(a_from, a_to, a_up);
	worldTransform = glm::inverse(viewTransform);
}
void Camera::SetPosition(glm::vec3 a_position)
{
	worldTransform = glm::translate(a_position);
	viewTransform = glm::inverse(worldTransform);
}
glm::mat4 Camera::GetWorld()
{
	glm::mat4 temp = worldTransform;
	temp[1] *= -1;
	temp[3].xyz = GetPosition();
	return temp;
}
//glm::mat4 Camera::GetView()				{ return viewTransform; }
glm::mat4 Camera::GetView()				{ return glm::inverse(worldTransform); }
glm::mat4 Camera::GetProjection()		{ return projectionTransform; }
glm::mat4 Camera::GetProjectionView()	{ return projectionViewTransform; }
glm::vec3 Camera::GetPosition()			{ return worldTransform[3].xyz; }
glm::vec3 Camera::GetUp()				{ return worldTransform[1].xyz; }
glm::vec3 Camera::GetForward()			{ return glm::vec3(-worldTransform[2].x, -worldTransform[2].y, -worldTransform[2].z); }
glm::vec3 Camera::GetRight()			{ return worldTransform[0].xyz; }

void Camera::GetFrustumPlanes()
{
	// right side
	frustumPlanes[0] = glm::vec4(projectionViewTransform[0][3] - projectionViewTransform[1][0],
		projectionViewTransform[1][3] - projectionViewTransform[1][0],
		projectionViewTransform[2][3] - projectionViewTransform[2][0],
		projectionViewTransform[3][3] - projectionViewTransform[3][0]);
	// left side
	frustumPlanes[1] = glm::vec4(projectionViewTransform[0][3] + projectionViewTransform[0][0],
		projectionViewTransform[1][3] + projectionViewTransform[1][0],
		projectionViewTransform[2][3] + projectionViewTransform[2][0],
		projectionViewTransform[3][3] + projectionViewTransform[3][0]);
	// top
	frustumPlanes[2] = glm::vec4(projectionViewTransform[0][3] - projectionViewTransform[0][1],
		projectionViewTransform[1][3] - projectionViewTransform[1][1],
		projectionViewTransform[2][3] - projectionViewTransform[2][1],
		projectionViewTransform[3][3] - projectionViewTransform[3][1]);
	// bottom
	frustumPlanes[3] = glm::vec4(projectionViewTransform[0][3] + projectionViewTransform[0][1],
		projectionViewTransform[1][3] + projectionViewTransform[1][1],
		projectionViewTransform[2][3] + projectionViewTransform[2][1],
		projectionViewTransform[3][3] + projectionViewTransform[3][1]);
	// far
	frustumPlanes[4] = glm::vec4(projectionViewTransform[0][3] - projectionViewTransform[0][2],
		projectionViewTransform[1][3] - projectionViewTransform[1][2],
		projectionViewTransform[2][3] - projectionViewTransform[2][2],
		projectionViewTransform[3][3] - projectionViewTransform[3][2]);
	// near
	frustumPlanes[5] = glm::vec4(projectionViewTransform[0][3] + projectionViewTransform[0][2],
		projectionViewTransform[1][3] + projectionViewTransform[1][2],
		projectionViewTransform[2][3] + projectionViewTransform[2][2],
		projectionViewTransform[3][3] + projectionViewTransform[3][2]);
	for (int i = 0; i < 6; i++) {
		frustumPlanes[i] = glm::normalize(frustumPlanes[i]);
	}
	//return &frustumPlanes;
}

// FLY Camera
FlyCamera::FlyCamera(const unsigned int a_ID)
{
	movementSpeed = 10.0f;
	rotationSpeed = 2.0f;
	cameraID = a_ID;
	isActive = true;
}

void FlyCamera::Update(float a_deltaTime)
{
	Camera::Update(a_deltaTime);
	//frustumPlanes = GetFrustumPlanes(); 

	if (!isSelected)
	{
		//glm::mat4 temp = this->getWorldTransform();
		//temp *= -1;
		//temp[3].xyz = this->getPosition();
		Gizmos::addTransform(this->GetWorld(), 1.0f);
		Gizmos::addSphere(this->GetPosition(), 1.0f, 8, 8, color.Clear);
		Gizmos::addLine(this->GetPosition(), this->GetPosition() + this->GetForward() * 3, color.Blue);
	}
	CheckInput(a_deltaTime);
}

void FlyCamera::CheckInput(float a_deltaTime)
{
	if (isSelected)
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
			glm::vec3 camera_right = (glm::vec3)worldTransform[0];

			glm::mat4 yaw = glm::rotate((float)x_delta, glm::vec3(0, 1, 0));
			glm::mat4 pitch = glm::rotate((float)y_delta, camera_right);
			glm::mat4 rot = yaw * pitch;

			worldTransform[0] = rot * worldTransform[0];
			worldTransform[1] = rot * worldTransform[1];
			worldTransform[2] = rot * worldTransform[2];
		}
		if ( glfwGetKey( window, GLFW_KEY_W) == GLFW_PRESS ) {
			worldTransform[3] -= worldTransform[2] * movementSpeed * a_deltaTime;
		} 
		if ( glfwGetKey( window, GLFW_KEY_S) == GLFW_PRESS ) {
			worldTransform[3] += worldTransform[2] * movementSpeed * a_deltaTime;
		}
		if ( glfwGetKey( window, GLFW_KEY_A) == GLFW_PRESS ) {
			worldTransform[3] -= worldTransform[0] * movementSpeed * a_deltaTime;
		}
		if ( glfwGetKey( window, GLFW_KEY_D) == GLFW_PRESS ) {
			worldTransform[3] += worldTransform[0] * movementSpeed * a_deltaTime;
		}
		if ( glfwGetKey( window, GLFW_KEY_Q) == GLFW_PRESS ) {
			worldTransform[3] -= worldTransform[1] * movementSpeed * a_deltaTime;
		}
		if ( glfwGetKey( window, GLFW_KEY_E) == GLFW_PRESS ) {
			worldTransform[3] += worldTransform[1] * movementSpeed * a_deltaTime;
		}
		if ( glfwGetKey( window, GLFW_KEY_SPACE) == GLFW_PRESS ) {
			worldTransform[3].y += movementSpeed * a_deltaTime;
		}
	}
}


void RenderTargetCamera::Update(float a_deltaTime)
{
	if (target != nullptr) {
		// move to be inverse of target Camera
		vec3 position;
		position = target->GetPosition() + target->GetForward() * 3;
		this->SetPosition(position);
	}
}
void RenderTargetCamera::SetTargetCamera(Camera* a_Camera)
{
	if (a_Camera != nullptr) {
		target = a_Camera;
	}
}