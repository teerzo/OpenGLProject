#ifndef _SCENE_MANAGEMENT_H_
#define _SCENE_MANAGEMENT_H_

#include "Application.h"
#include "BoundingShapes.h"

class SceneManagement : public Application
{
private:
	// Base Variables
	glm::vec4 plane;
	// Project Specific

public:
	virtual ~SceneManagement();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 

};

#endif // _SCENE_MANAGEMENT_H_