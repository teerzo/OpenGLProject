#ifndef _TEMPLATEAPPLICATION_H_
#define _TEMPLATEAPPLICATION_H_

#include "Application.h"

class TemplateApplication : public Application
{
private:
	float timer;
	//glm::mat4 ProjectionMatrix;
	//glm::mat4 ViewMatrix;

public:
	virtual ~TemplateApplication();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

};

#endif // _CAMERAANDPROJECTIONS_H_