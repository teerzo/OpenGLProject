#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "glm_header.h"

#include <vector>

class Camera;

struct Screen
{	
	int Width;
	int Height;
};


struct GLFWwindow;

class Application
{
private:

protected:
	std::vector<Camera*> m_vListofCameras;
	char* AppName;
	Screen ScreenSize;
	Color color;
	GLFWwindow* window;
public:
	Application();
	virtual ~Application();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	GLFWwindow* getWindow();
};


#endif // _APPLICATION_H_