#ifndef _TEMPLATEAPPLICATION_H_
#define _TEMPLATEAPPLICATION_H_

#include "Application.h"

class TemplateApplication : public Application
{
private:
	// Base Variables

	// Project Specific

public:
	virtual ~TemplateApplication();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 

};

#endif // _TEMPLATEAPPLICATION_H_