#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "glm_header.h"
//#include "../deps/AntTweakBar/include/AntTweakBar.h"
#include "AntTweakBar.h"

#include "Vertex.h"

#include <vector>

#include "Utility.h"

class Camera;
class Line;





struct Mode
{
	bool LockCamera;
	bool Debug;
	bool Wireframe;
	bool ShowObjects;
	bool ShowLines;
	bool CastLines;
	bool ShowGrid;
	bool ShowUI;
};

//struct OpenGLData
//{
//	// Vertex Array Object
//	unsigned int m_VAO;
//	// Vertex Buffer Object
//	unsigned int m_VBO;
//	// Index Buffer Object
//	unsigned int m_IBO;
//	unsigned int m_index_count;
//};

struct GLFWwindow;

class Application
{
private:

	void _UpdateLines();
	void _ClearLines();

	void _UpdateCameras();
	void _CheckKeys();
	void _DisableOtherCameras();

	// Project Default Variables
	float m_fps;

	// Debug Variables
	int _ui_GridSize;
	std::vector<Line*> d_vec_RayLines;
protected:

	// Grid Functions
	void _DrawGrid();
	// Line Functions 
	void _DrawLines();

	std::vector<Camera*> m_vListofCameras;
	signed int ActiveCamera;
	char* AppName;
	Screen ScreenSize;
	Mode mode;
	GLFWwindow* window;

	OpenGLData m_quad;
	OpenGLData m_grid;

	glm::vec4 m_BackGroundColor;

	// Tweak Bar
	TwBar* m_bar;

	// timer/deltatime
	float m_fTimer;
	float m_fPreviousTime;
	float m_fdeltaTime;
	float m_fDelayMax;
	float m_fDelayTimer;

	// Testing Time Wave
	float m_sin_wave;
	// Textures // 
	unsigned int m_Texture;
	
	unsigned int m_ProgramID;	
	unsigned int m_Basic_Program;

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


	// Tweak Bar
};


#endif // _APPLICATION_H_