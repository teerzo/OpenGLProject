#ifndef _CAMERA_H_
#define _CAMERA_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>

class Camera
{
public:
	bool			isActive; // Updates Camera if active
	bool			isSelected; // Moves Camera if selected
	float			movementSpeed;
	float			rotationSpeed;
	glm::mat4		worldTransform;
	glm::mat4		viewTransform;
	glm::mat4		projectionTransform;
	glm::mat4		projectionViewTransform;
	std::vector<glm::vec4>frustumPlanes;

	Camera();
	void DestroyCamera();
	virtual void Update(float a_deltaTime);
	void SetViewProjection(glm::mat4 a_Matrix);
	void SetPerspective(float a_FOV, float a_aspectRatio, float a_near, float a_far);
	void SetLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up);
	void SetPosition(glm::vec3 a_position);
	glm::mat4 GetWorld();
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::mat4 GetProjectionView();
	glm::vec3 GetPosition();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
	void GetFrustumPlanes();
protected:
	unsigned int cameraID;
	void UpdateProjectionView();
};

class FlyCamera : public Camera
{
public:
	FlyCamera(unsigned int a_CameraID);
	void Update(float a_deltaTime);
	void CheckInput(float a_deltaTime);
	
};

class RenderTargetCamera : public Camera
{
public:
	Camera* target;
	void Update(float a_deltaTime);
	void SetTargetCamera(Camera* a_Camera);

};




#endif // _CAMERA_H_
