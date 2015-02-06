#ifndef _CAMERAANDPROJECTIONS_H_
#define _CAMERAANDPROJECTIONS_H_

#include "Application.h"

class Camera;
class FlyCamera;

class CameraAndProjections : public Application
{
private:
	float previousTime;
	glm::mat4 ProjectionMatrix;
	//Camera* sceneCamera;
	//FlyCamera* flyCamera;

public:
	virtual ~CameraAndProjections();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

};

#endif // _CAMERAANDPROJECTIONS_H_