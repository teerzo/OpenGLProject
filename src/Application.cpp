#include "Application.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Utility.h"
#include "Gizmos.h"
#include "Line.h"
#include "Camera.h"
#include "BoundingShapes.h"

void OnMouseButton(GLFWwindow* window, int button, int pressed, int mod_keys)
{
	TwEventMouseButtonGLFW(button, pressed);
}
void OnMousePosition(GLFWwindow* a_window, double x, double y)
{
	TwEventMousePosGLFW((int)x, (int)y);
}
void OnMouseScroll(GLFWwindow* window, double x, double y)
{
	TwEventMouseWheelGLFW((int)y);
}
void OnKey(GLFWwindow* window, int key, int scancode, int pressed, int mod_keys)
{
	TwEventKeyGLFW(key, pressed);
}
void OnChar(GLFWwindow* window, unsigned int ch)
{
	TwEventCharGLFW(ch, GLFW_PRESS);
}
void OnWindowResize(GLFWwindow* window, int width, int height)
{
	TwWindowSize(width, height);
	glViewport(0, 0, width, height);
}


void TW_CALL ClearLines()
{
	
}


Application::Application()
{

}

Application::~Application()
{

}


void Application::SetApplicationDefaults()
{
	this->applicationName = "Application Default";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
	
}

bool Application::ApplicationStartup()
{
	if (!glfwInit())
	{
		return false;
	}
	printf("glfwInit Successful\n");
	this->window = glfwCreateWindow(this->screenSize.Width, this->screenSize.Height, this->applicationName, nullptr, nullptr);
	if (this->window == nullptr)
	{
		return false;
	}
	printf("glfwCreateWindow Successful\n");
	glfwMakeContextCurrent(this->window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(this->window);
		glfwTerminate();
		return false;
	}
	int Major_Version = ogl_GetMajorVersion();
	int Minor_Version = ogl_GetMinorVersion();

	printf("Opengl Version: %d.%d Loaded\n", Major_Version, Minor_Version);

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(this->screenSize.Width, this->screenSize.Height);
	tweakBarMain = TwNewBar("Main");
	tweakBarCamera = TwNewBar("Cameras");

	printf("TweakBar Loaded\n");

	glfwSetMouseButtonCallback(this->window, OnMouseButton);
	glfwSetCursorPosCallback(this->window, OnMousePosition);
	glfwSetScrollCallback(this->window, OnMouseScroll);
	glfwSetKeyCallback(this->window, OnKey);
	glfwSetCharCallback(this->window, OnChar);
	glfwSetWindowSizeCallback(this->window, OnWindowResize);

	if (!LoadShader(&basicProgram, "../data/shaders/basic_vertex.glsl", "../data/shaders/basic_fragment.glsl", nullptr ))
	{
		return false;
	}

	debugModes.lockCamera = false;
	debugModes.Debug = true;
	debugModes.Wireframe = false;
	debugModes.ShowLines = true;
	debugModes.ShowGrid = true;
	debugModes.CastLines = false;

	m_fDelayTimer = 0.0f;
	m_fDelayMax = 1.0f;

	Gizmos::create();
	glfwSetTime(0.0);

	defaultBackgroundColour = glm::vec4(0.2, 0.2, 0.2, 1);

	debugGridSize = 1000;

	// TweakBar Variables
	//TwAddSeparator(m_bar, "General", "");
	TwAddVarRO(tweakBarMain, "FPS ", TW_TYPE_FLOAT, &FPS, "group=General precision=2");
	TwAddVarRW(tweakBarMain, "Clear Colour", TW_TYPE_COLOR4F, &defaultBackgroundColour, "group=General");
	TwAddVarRW(tweakBarMain, "Debug ", TW_TYPE_BOOL8, &debugModes.Debug, "group=Modes");
	TwAddVarRW(tweakBarMain, "Show Grid ", TW_TYPE_BOOL8, &debugModes.ShowGrid, "group=Grid");
	TwAddVarRW(tweakBarMain, "Size ", TW_TYPE_INT32, &debugGridSize, "group=Grid");

	TwAddVarRW(tweakBarMain, "Show Lines ", TW_TYPE_BOOL8, &debugModes.ShowLines, "group=Lines");
	TwAddVarRW(tweakBarMain, "Cast Lines ", TW_TYPE_BOOL8, &debugModes.CastLines, "group=Lines");
	//TwAddButton(m_bar, "Clear Lines", ClearLines )
	//TwAddVarRW(m_bar, "Max Length ", TW_TYPE_INT32, &_ui_GridSize, "group=Grid");

	//TwAddVarRW(m_bar, "Speed", TW_TYPE_FLOAT, &m_vListofCameras[0]->m_fSpeed, "group=Camera0");

	currentCamera = 0;
	AddFlyCamera();

	printf("************\n%s Init Complete\n", this->applicationName);
	return true;
}

void Application::ApplicationShutdown()
{
	if (this->window != nullptr)
	{
		glfwDestroyWindow(this->window);
	}
	// Close TweakBar
	TwDeleteAllBars();
	TwTerminate();

	// Close Gizmos
	Gizmos::destroy();
	glfwTerminate();
}

bool Application::Update()
{
	if (glfwWindowShouldClose(this->window))
	{
		return false;
	}

	currentGameTime = (float)glfwGetTime();
	deltaTime = currentGameTime - previousGameTime; // prev of last frame
	previousGameTime = currentGameTime;

	FPS = 1 / deltaTime;

	m_sin_wave = sinf(currentGameTime * 5) * 0.5f + 0.5f;


	if (m_fDelayTimer < m_fDelayMax)
	{
		m_fDelayTimer += deltaTime;
	}

	Gizmos::clear();
	DebugUpdateLines();
	UpdateCameras();
	CheckInput();

	return true;
}

void Application::DebugDraw()
{
	DebugDrawGrid();
	DebugDrawLines();
	Gizmos::draw(cameraVector[currentCamera]->GetProjectionView());
}

void Application::Draw()
{
	TwDraw(); // Draw Tweak Bar
}

GLFWwindow* Application::GetWindow()
{
	return this->window;
}

void Application::DebugDrawGrid()
{
	if (debugModes.Debug)
	{
		if (debugModes.ShowGrid)
		{
			for (int i = 0; i <= debugGridSize; ++i)
			{
				Gizmos::addLine(glm::vec3(-(debugGridSize / 2) + i, 0, -(debugGridSize / 2)), glm::vec3(-(debugGridSize / 2) + i, 0, (debugGridSize / 2)), i == (debugGridSize / 2) ? color.White : color.Gray);
				Gizmos::addLine(glm::vec3(-(debugGridSize / 2), 0, -(debugGridSize / 2) + i), glm::vec3((debugGridSize / 2), 0, -(debugGridSize / 2) + i), i == (debugGridSize / 2) ? color.White : color.Gray);
			}
		}
	}
}

void Application::DebugDrawLines()
{
	if (debugModes.Debug)
	{
		if (debugModes.ShowLines)
		{
			for (unsigned int i = 0; i < debugLinesVector.size(); ++i)
			{
				debugLinesVector[i]->Draw();
			}
		}
	}
}

void Application::DebugUpdateLines()
{
	for (unsigned int i = 0; i < debugLinesVector.size(); ++i)
	{
		debugLinesVector[i]->Update();
	}
}

void Application::DebugClearLines()
{
	debugLinesVector.clear();
}

void Application::UpdateCameras()
{
	cameraVector[currentCamera]->Update(deltaTime);
	for (unsigned i = 0; i < cameraVector.size(); ++i)
	{
		if (cameraVector[i]->isActive)
		{
			cameraVector[i]->Update(deltaTime);
		}
		if (debugModes.Debug)
		{
			
		}
	}
}

void Application::CheckInput()
{
	// hotkeys.... move to own function
	if (glfwGetMouseButton(window, 0) && debugModes.CastLines)
	{
		if (m_fDelayTimer >= m_fDelayMax )
		{
			m_fDelayTimer = 1.0f;
			debugLinesVector.push_back(new Line(cameraVector[currentCamera]->GetPosition(), cameraVector[currentCamera]->GetForward()));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			currentCamera = 0;
			m_fDelayTimer = 0.0f;
			DisableOtherCameras();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		if (currentCamera - 1 >= 0)
		{
			if (m_fDelayTimer >= m_fDelayMax)
			{
				currentCamera -= 1;
				m_fDelayTimer = 0.0f;
				DisableOtherCameras();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		if ((unsigned int)currentCamera + 1 < cameraVector.size())
		{
			if (m_fDelayTimer >= m_fDelayMax)
			{
				currentCamera += 1;
				m_fDelayTimer = 0.0f;
				DisableOtherCameras();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			AddFlyCamera();
			m_fDelayTimer = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			DestroyActiveCamera();
			m_fDelayTimer = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		debugModes.Debug = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		debugModes.Debug = false;
	}
}

void Application::DisableOtherCameras()
{
	if ((unsigned int)currentCamera < cameraVector.size())
	{
		if (!debugModes.lockCamera)
		{
			cameraVector[currentCamera]->isSelected = true;
			printf("~Selected Camera : %d\n", currentCamera);
		}
		for (unsigned int i = 0; i < cameraVector.size(); ++i)
		{
			if (i != currentCamera)
			{
				cameraVector[i]->isSelected = false;
			}
		}
	}
}

void Application::AddFlyCamera()
{	
	this->cameraVector.push_back(new FlyCamera(cameraVector.size()));
	currentCamera = cameraVector.size() - 1;
	std::string _stringCamera = "group=Camera" + std::to_string(currentCamera);
	std::string _stringMovementSpeed = std::to_string(currentCamera) + " Move";
	std::string _stringRotationSpeed = std::to_string(currentCamera) + " Rotate";
	char const* _MovementSpeed = _stringMovementSpeed.c_str();
	char const* _RotationSpeed = _stringRotationSpeed.c_str();
	char const* group = _stringCamera.c_str();
	TwAddVarRW(tweakBarCamera, _MovementSpeed, TW_TYPE_FLOAT, &cameraVector[currentCamera]->movementSpeed, group);
	TwAddVarRW(tweakBarCamera, _RotationSpeed, TW_TYPE_FLOAT, &cameraVector[currentCamera]->rotationSpeed, group);
	DisableOtherCameras();
}

void Application::DestroyActiveCamera()
{
	if (this->cameraVector.size() > 1)
	{
		// delete active camera
		//this->m_vListofCameras.
	}
}