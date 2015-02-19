#ifndef _QUATERNIONS_H_
#define _QUATERNIONS_H_

#include "Application.h"

struct KeyFrame
{
	glm::vec3 position;
	glm::quat rotation;

};

class Quaternions : public Application
{
private:
	// Base Variables

	// Project Specific

	KeyFrame m_hip_frames[2];
	KeyFrame m_knee_frames[2];
	KeyFrame m_ankle_frames[2];
	KeyFrame m_foot_frames[2];

	glm::mat4 m_hip_bone;
	glm::mat4 m_knee_bone;
	glm::mat4 m_ankle_bone;
	glm::mat4 m_foot_bone;


public:
	virtual ~Quaternions();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 


};

#endif // _QUATERNIONS_H_