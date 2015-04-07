#ifndef _TEMPLATEAPPLICATION_H_
#define _TEMPLATEAPPLICATION_H_

#include "Application.h"

class TemplateApplication : public Application
{
private:
	
public:
	virtual ~TemplateApplication();
	
	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual void DebugDraw();

};

#endif // _TEMPLATEAPPLICATION_H_