#ifndef _CAMERAANDPROJECTIONS_H_
#define _CAMERAANDPROJECTIONS_H_

#include "Application.h"
#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class CameraAndProjections : public Application
{
private:
	float timer;
	glm::mat4 ProjectionMatrix;

public:
	virtual ~CameraAndProjections();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

};

#endif // _CAMERAANDPROJECTIONS_H_