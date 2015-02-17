#ifndef _CAMERAANDPROJECTIONS_H_
#define _CAMERAANDPROJECTIONS_H_

#include "Application.h"

class CameraAndProjections : public Application
{
private:
	// Base Variables

	// Project Specific
	
public:
	virtual ~CameraAndProjections();
	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 

};

#endif // _CAMERAANDPROJECTIONS_H_