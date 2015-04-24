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
	this->screen_size.width = 1280;
	this->screen_size.height = 720;

}

bool Application::ApplicationStartup()
{
	if (!glfwInit())
	{
		return false;
	}
	printf("glfwInit Successful\n");
	this->window = glfwCreateWindow(this->screen_size.width, this->screen_size.height, this->applicationName, nullptr, nullptr);
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
	TwWindowSize(this->screen_size.width, this->screen_size.width);
	tweakBarMain = TwNewBar("Main");
	tweakBarCamera = TwNewBar("Cameras");

	printf("TweakBar Loaded\n");

	glfwSetMouseButtonCallback(this->window, OnMouseButton);
	glfwSetCursorPosCallback(this->window, OnMousePosition);
	glfwSetScrollCallback(this->window, OnMouseScroll);
	glfwSetKeyCallback(this->window, OnKey);
	glfwSetCharCallback(this->window, OnChar);
	glfwSetWindowSizeCallback(this->window, OnWindowResize);

	if (!LoadShader(&basicProgram, "basic_vertex.glsl", "basic_fragment.glsl", nullptr))
	{
		return false;
	}

	mode.debug = true;
	mode.wireframe = false;

	m_fDelayTimer = 0.0f;
	m_fDelayMax = 1.0f;

	Gizmos::create();
	glfwSetTime(0.0);

	bgColor = glm::vec4(0.2, 0.2, 0.2, 1);

	debugGridSize = 100;

	// TweakBar Variables
	TwAddVarRO(tweakBarMain, "FPS ", TW_TYPE_FLOAT, &time.FPS, "group=General precision=2");
	TwAddVarRW(tweakBarMain, "Clear Colour", TW_TYPE_COLOR4F, &bgColor, "group=General");
	TwAddVarRW(tweakBarMain, "Debug ", TW_TYPE_BOOL8, &mode.debug, "group=Modes");
	TwAddVarRW(tweakBarMain, "Show Grid ", TW_TYPE_BOOL8, &mode.show_grid, "group=Grid");
	TwAddVarRW(tweakBarMain, "Size ", TW_TYPE_INT32, &debugGridSize, "group=Grid");

	currentCamera = 0;
	AddCamera();

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

	time.current = (float)glfwGetTime();
	time.delta_time = time.current - time.previous; // prev of last frame
	time.previous = time.current;

	time.FPS = 1 / time.delta_time;

	m_sin_wave = sinf(time.current * 5) * 0.5f + 0.5f;


	if (m_fDelayTimer < m_fDelayMax)
	{
		m_fDelayTimer += time.delta_time;
	}

	Gizmos::clear();
	UpdateCameras();
	CheckInput();

	return true;
}

void Application::DebugDraw()
{
	DebugDrawGrid();
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
	if (mode.debug)
	{
		if (mode.show_grid)
		{
			for (int i = 0; i <= debugGridSize; ++i)
			{
				Gizmos::addLine(glm::vec3(-(debugGridSize / 2) + i, 0, -(debugGridSize / 2)), glm::vec3(-(debugGridSize / 2) + i, 0, (debugGridSize / 2)), i == (debugGridSize / 2) ? color.White : color.Gray);
				Gizmos::addLine(glm::vec3(-(debugGridSize / 2), 0, -(debugGridSize / 2) + i), glm::vec3((debugGridSize / 2), 0, -(debugGridSize / 2) + i), i == (debugGridSize / 2) ? color.White : color.Gray);
			}
		}
	}
}

void Application::UpdateCameras()
{
	cameraVector[currentCamera]->Update(time.delta_time);
	for (unsigned i = 0; i < cameraVector.size(); ++i)
	{
		cameraVector[i]->Update(time.delta_time);
	}
}

void Application::CheckInput()
{
	// hotkeys.... move to own function
	
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
			AddCamera();
			m_fDelayTimer = 0.0f;
		}
	}
}

void Application::DisableOtherCameras()
{
	if ((unsigned int)currentCamera < cameraVector.size())
	{
		for (unsigned int i = 0; i < cameraVector.size(); ++i)
		{
			if (i != currentCamera)
			{
				cameraVector[i]->is_selected = false;
			}
		}
	}
}

void Application::AddCamera()
{
	this->cameraVector.push_back(new Camera(screen_size));
	currentCamera = cameraVector.size() - 1;
	std::string _stringCamera = "group=Camera" + std::to_string(currentCamera);
	std::string _stringMovementSpeed = std::to_string(currentCamera) + " Move";
	std::string _stringRotationSpeed = std::to_string(currentCamera) + " Rotate";
	char const* _MovementSpeed = _stringMovementSpeed.c_str();
	char const* _RotationSpeed = _stringRotationSpeed.c_str();
	char const* group = _stringCamera.c_str();
	TwAddVarRW(tweakBarCamera, _MovementSpeed, TW_TYPE_FLOAT, &cameraVector[currentCamera]->speed_movement, group);
	TwAddVarRW(tweakBarCamera, _RotationSpeed, TW_TYPE_FLOAT, &cameraVector[currentCamera]->speed_rotation, group);
	DisableOtherCameras();
}
