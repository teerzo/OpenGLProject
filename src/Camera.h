#ifndef _CAMERA_H_
#define _CAMERA_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
{
protected:
	glm::mat4 m_mWorldTransform;
	glm::mat4 m_mViewTransform;
	glm::mat4 m_mProjectionTransform;
	glm::mat4 m_mProjectionViewTransform;

	void updateProjectionViewTransform();
public:
	Camera();
	virtual void update(float a_deltaTime);
	void setPerspective(float a_FOV, float a_aspectRatio, float a_near, float a_far);
	void setLookAt(glm::vec3 a_from, glm::vec3 a_to, glm::vec3 a_up);
	void setPosition(glm::vec3 a_position);
	glm::mat4 getWorldTransform();
	glm::mat4 getViewTransform();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();
	glm::vec3 getPosition();
	glm::vec3 getForward();
	glm::vec3 getRight();


	bool m_bIsSelected;
};

class FlyCamera : public Camera
{
private:
	float m_fSpeed;
	glm::vec3 m_vUp;
	

public:
	FlyCamera();
	void update(float a_deltaTime);
	void setSpeed(float a_speed);
	float getSpeed();
	
};

class SpotLightCamera : public FlyCamera
{
private:
	//float m_fSpeed;
	//glm::vec3 m_vUp;


public:
	SpotLightCamera();
	//void update(float a_deltaTime);
	//void setSpeed(float a_speed);
	//float getSpeed();

};

#endif // _CAMERA_H_
