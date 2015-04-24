#ifndef _CAMERA_H_
#define _CAMERA_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#include "Object.h"

struct Screen;
class Camera : public Object
{
public:
	bool			is_selected; // Moves Camera if selected
	float			speed_movement;
	float			speed_rotation;

	// Inherit world and local matrix
	// glm::mat4	world_matrix;
	// glm::mat4	local_matrix;
	glm::mat4		matrix_view;
	glm::mat4		matrix_projection;
	glm::mat4		matrix_projection_view;

	Camera( Screen screenres );
	void Update(const float delta_time);
	void CheckInput(const float delta_time);

	void SetViewProjection(const glm::mat4 a_Matrix);
	void SetPerspective(const float a_FOV, const float a_aspectRatio, const float a_near, const float a_far);
	void SetLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up);

	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::mat4 GetProjectionView();
	
	unsigned int cameraID;
};

class RenderTargetCamera : public Camera
{
public:
	Camera* target;
	void Update(float a_deltaTime);
	void SetTargetCamera(Camera* a_Camera);
};

#endif // _CAMERA_H_
