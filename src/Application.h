#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include <vector>

class Camera;

struct Screen
{	
	int Width;
	int Height;
};

struct Color
{
	glm::vec4 White = glm::vec4(1, 1, 1, 1);
	glm::vec4 Black = glm::vec4(0, 0, 0, 1);
	glm::vec4 Gray = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	glm::vec4 Red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 Blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

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