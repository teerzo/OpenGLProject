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

struct Mode
{
	bool Debug;
	bool Wireframe;
};

struct GLFWwindow;

class Application
{
private:

protected:
	std::vector<Camera*> m_vListofCameras;
	int ActiveCamera;
	char* AppName;
	Screen ScreenSize;
	Color color;
	Mode mode;
	GLFWwindow* window;

	// timer/deltatime
	float m_fTimer;
	float m_fPreviousTime;
	float m_fdeltaTime;

	// Textures // 
	unsigned int m_Texture;
	// Vertex Array Object
	unsigned int m_VAO;
	// Vertex Buffer Object
	unsigned int m_VBO;
	// Index Buffer Object
	unsigned int m_IBO;
	// Combined Program Object, Contains m_VBO/m_IBO etc
	unsigned int m_ProgramID;

	// Shaders // 
	const char* vertexShader_Source;
	const char* fragmentShader_Source;

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