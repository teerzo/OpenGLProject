#ifndef _TEMPLATEAPPLICATION_H_
#define _TEMPLATEAPPLICATION_H_

#include "Application.h"
#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class TemplateApplication : public Application
{
private:
	float timer;
	glm::mat4 ProjectionMatrix;

public:
	virtual ~TemplateApplication();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

};

#endif // _CAMERAANDPROJECTIONS_H_