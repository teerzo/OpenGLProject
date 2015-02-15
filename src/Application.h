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
	bool LockCamera;
	bool Debug;
	bool Wireframe;
	bool HideObjects;
};

struct OpenGLData
{
	// Vertex Array Object
	unsigned int m_VAO;
	// Vertex Buffer Object
	unsigned int m_VBO;
	// Index Buffer Object
	unsigned int m_IBO;
	unsigned int m_index_count;
};

struct GLFWwindow;

class Application
{
private:
	void _DrawGrid();
	void _UpdateCameras();
	void _CheckKeys();
	void _DisableOtherCameras();

protected:
	std::vector<Camera*> m_vListofCameras;
	signed int ActiveCamera;
	char* AppName;
	Screen ScreenSize;
	Color color;
	Mode mode;
	GLFWwindow* window;

	OpenGLData m_quad;
	OpenGLData m_grid;

	// timer/deltatime
	float m_fTimer;
	float m_fPreviousTime;
	float m_fdeltaTime;
	float m_fDelayMax;
	float m_fDelayTimer;

	// Textures // 
	unsigned int m_Texture;
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
	//virtual void checkKeys();

	void AddFlyCamera();
	void DestroyActiveCamera();

	GLFWwindow* getWindow();
};


#endif // _APPLICATION_H_