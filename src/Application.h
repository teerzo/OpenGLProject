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

struct km_time
{
	float delta_time;
	float current;
	float previous;
	float FPS;
};

struct km_mode
{
	bool debug;
	bool show_grid;
	bool wireframe;
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

	void AddCamera();
	GLFWwindow* GetWindow();	
private:
	void UpdateCameras();
	void DisableOtherCameras();
	//void teCheckInput();

	

	// Debug Variables
	int debugGridSize;
	std::vector<Line*> debugLinesVector;
protected:
	std::vector<Camera*> cameraVector;
	signed int currentCamera;
	char* applicationName;
	Screen screen_size;
	GLFWwindow* window;

	km_time time;
	km_mode mode;

	// Grid Functions
	void DebugDrawGrid();
	
	glm::vec4 bgColor;

	// Tweak Bar
	TwBar* tweakBarMain;
	TwBar* tweakBarCamera;

	// timer/deltatime

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