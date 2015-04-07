#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "glm_header.h"
#include "AntTweakBar.h"
#include "Vertex.h"
#include <vector>
#include "Utility.h"

#include "Mesh.h"

class Camera;
class Line;

struct debugModes_t
{
	bool lockCamera;
	bool Debug;
	bool Wireframe;
	bool ShowObjects;
	bool ShowLines;
	bool CastLines;
	bool ShowGrid;
	bool ShowUI;
};

struct GLFWwindow;
class Application
{
public:
	Application();
	virtual ~Application();

	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();	
	virtual	void DebugDraw();

	Screen GetScreenSize();
	void AddFlyCamera();
	void DestroyActiveCamera();
	GLFWwindow* GetWindow();	
private:
	void UpdateCameras();
	void DisableOtherCameras();
	//void teCheckInput();

	// Project Default Variables
	float FPS;

	// Debug Variables
	int debugGridSize;
	std::vector<Line*> debugLinesVector;
protected:
	std::vector<Camera*> cameraVector;
	signed int currentCamera;
	char* applicationName;
	Screen screenSize;
	debugModes_t debugModes;
	GLFWwindow* window;

	// Grid Functions
	void DebugDrawGrid();
	// Line Functions 
	void DebugUpdateLines();
	void DebugDrawLines();
	void DebugClearLines();

	//OpenGLData m_quad;
	//OpenGLData m_grid;

	glm::vec4 defaultBackgroundColour;

	// Tweak Bar
	TwBar* tweakBarMain;
	TwBar* tweakBarCamera;

	// timer/deltatime
	float currentGameTime;
	float previousGameTime;
	float deltaTime;
	float m_fDelayMax;
	float m_fDelayTimer;

	// Testing Time Wave
	float m_sin_wave;
	// Textures // 
	//unsigned int m_Texture;
	
	unsigned int basicProgram;
	unsigned int m_ProgramID;
};


#endif // _APPLICATION_H_