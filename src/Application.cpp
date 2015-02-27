#include "Application.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Utility.h"
#include "Gizmos.h"

#include "Line.h"

#include "Camera.h"

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
void Application::setDefaults()
{
	this->AppName = "Application Default";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;
	
}

bool Application::startup()
{
	if (!glfwInit())
	{
		return false;
	}
	printf("glfwInit Successful\n");
	this->window = glfwCreateWindow(this->ScreenSize.Width, this->ScreenSize.Height, this->AppName, nullptr, nullptr);
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
	TwWindowSize(this->ScreenSize.Width, this->ScreenSize.Height);
	m_bar = TwNewBar("TweakBar");
	printf("TweakBar Loaded\n");

	glfwSetMouseButtonCallback(this->window, OnMouseButton);
	glfwSetCursorPosCallback(this->window, OnMousePosition);
	glfwSetScrollCallback(this->window, OnMouseScroll);
	glfwSetKeyCallback(this->window, OnKey);
	glfwSetCharCallback(this->window, OnChar);
	glfwSetWindowSizeCallback(this->window, OnWindowResize);


	// load default shader
	//if (!LoadShader("basic", (GLuint*)&m_Basic_Program))
	if (!LoadShader((GLuint*)&m_Basic_Program, "./data/shaders/basic_vertex.glsl", "../data/shaders/basic_fragment.glsl", nullptr ))
	{
		return false;
	}

	mode.LockCamera = false;
	mode.Debug = true;
	mode.Wireframe = false;
	mode.ShowLines = true;
	mode.ShowGrid = true;
	mode.CastLines = false;

	m_fDelayTimer = 0.0f;
	m_fDelayMax = 1.0f;


	m_BackGroundColor = glm::vec4(0.2, 0.2, 0.2, 1);

	_ui_GridSize = 1000;

	// TweakBar Variables
	//TwAddSeparator(m_bar, "General", "");
	TwAddVarRO(m_bar, "Fps ", TW_TYPE_FLOAT, &m_fps, "group=General precision=2");
	TwAddVarRW(m_bar, "Clear Colour", TW_TYPE_COLOR4F, &m_BackGroundColor, "group=General");
	TwAddVarRW(m_bar, "Debug ", TW_TYPE_BOOL8, &mode.Debug, "group=Modes");
	TwAddVarRW(m_bar, "Show Grid ", TW_TYPE_BOOL8, &mode.ShowGrid, "group=Grid");
	TwAddVarRW(m_bar, "Size ", TW_TYPE_INT32, &_ui_GridSize, "group=Grid");

	TwAddVarRW(m_bar, "Show Lines ", TW_TYPE_BOOL8, &mode.ShowLines, "group=Lines");
	TwAddVarRW(m_bar, "Cast Lines ", TW_TYPE_BOOL8, &mode.CastLines, "group=Lines");
	//TwAddButton(m_bar, "Clear Lines", ClearLines )
	//TwAddVarRW(m_bar, "Max Length ", TW_TYPE_INT32, &_ui_GridSize, "group=Grid");

	//TwAddVarRW(m_bar, "Speed", TW_TYPE_FLOAT, &m_vListofCameras[0]->m_fSpeed, "group=Camera0");

	ActiveCamera = 0;
	AddFlyCamera();

	printf("************\n%s Init Complete\n", this->AppName);
	return true;
}

void Application::shutdown()
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

bool Application::update()
{
	if (glfwWindowShouldClose(this->window))
	{
		return false;
	}

	m_fTimer = (float)glfwGetTime();
	m_fdeltaTime = m_fTimer - m_fPreviousTime; // prev of last frame
	m_fPreviousTime = m_fTimer;

	m_fps = 1 / m_fdeltaTime;

	m_sin_wave = sinf(m_fTimer * 5) * 0.5f + 0.5f;


	if (m_fDelayTimer < m_fDelayMax)
	{
		m_fDelayTimer += m_fdeltaTime;
	}

	Gizmos::clear();


	_UpdateLines();

	_UpdateCameras();

	_CheckKeys();

	return true;
}

void Application::draw()
{
	// Base Draw Code
	_DrawGrid();
	_DrawLines();
	Gizmos::draw(m_vListofCameras[ActiveCamera]->getProjectionView());
	TwDraw();
}

GLFWwindow* Application::getWindow()
{
	return this->window;
}

void Application::_DrawGrid()
{
	if (mode.Debug)
	{
		if (mode.ShowGrid)
		{
			for (int i = 0; i <= _ui_GridSize; ++i)
			{
				Gizmos::addLine(glm::vec3(-(_ui_GridSize / 2) + i, 0, -(_ui_GridSize / 2)), glm::vec3(-(_ui_GridSize / 2) + i, 0, (_ui_GridSize / 2)), i == (_ui_GridSize / 2) ? color.White : color.Gray);
				Gizmos::addLine(glm::vec3(-(_ui_GridSize / 2), 0, -(_ui_GridSize / 2) + i), glm::vec3((_ui_GridSize / 2), 0, -(_ui_GridSize / 2) + i), i == (_ui_GridSize / 2) ? color.White : color.Gray);
			}
		}
	}
}

void Application::_DrawLines()
{
	if (mode.Debug)
	{
		if (mode.ShowLines)
		{
			for (unsigned int i = 0; i < d_vec_RayLines.size(); ++i)
			{
				d_vec_RayLines[i]->Draw();
			}
		}
	}
}

void Application::_UpdateLines()
{
	for (unsigned int i = 0; i < d_vec_RayLines.size(); ++i)
	{
		d_vec_RayLines[i]->Update();
	}
}

void Application::_ClearLines()
{
	d_vec_RayLines.clear();
}

void Application::_UpdateCameras()
{
	for (unsigned i = 0; i < m_vListofCameras.size(); ++i)
	{
		m_vListofCameras[i]->update(m_fdeltaTime);
		if (mode.Debug)
		{
			
		}
	}
}

void Application::_CheckKeys()
{
	// hotkeys.... move to own function
	if (glfwGetMouseButton(window, 0) && mode.CastLines)
	{
		if (m_fDelayTimer >= m_fDelayMax )
		{
			m_fDelayTimer = 0.0f;
			d_vec_RayLines.push_back(new Line(m_vListofCameras[ActiveCamera]->getPosition(), m_vListofCameras[ActiveCamera]->getForward()));
		}
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (m_fDelayTimer >= m_fDelayMax)
		{
			ActiveCamera = 0;
			m_fDelayTimer = 0.0f;
			_DisableOtherCameras();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		if (ActiveCamera - 1 >= 0)
		{
			if (m_fDelayTimer >= m_fDelayMax)
			{
				ActiveCamera -= 1;
				m_fDelayTimer = 0.0f;
				_DisableOtherCameras();
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		if ((unsigned int)ActiveCamera + 1 < m_vListofCameras.size())
		{
			if (m_fDelayTimer >= m_fDelayMax)
			{
				ActiveCamera += 1;
				m_fDelayTimer = 0.0f;
				_DisableOtherCameras();
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
		mode.Debug = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		mode.Debug = false;
	}
}

void Application::_DisableOtherCameras()
{
	if ((unsigned int)ActiveCamera < m_vListofCameras.size())
	{
		if (!mode.LockCamera)
		{
			m_vListofCameras[ActiveCamera]->m_bIsSelected = true;
			printf("~Active Camera : %d\n", ActiveCamera);
		}
		for (unsigned int i = 0; i < m_vListofCameras.size(); ++i)
		{
			if (i != ActiveCamera)
			{
				m_vListofCameras[i]->m_bIsSelected = false;
			}
		}
	}
}

void Application::AddFlyCamera()
{
	
	this->m_vListofCameras.push_back(new FlyCamera(m_vListofCameras.size()));
	ActiveCamera = m_vListofCameras.size() - 1;
	std::string tempG = "group=Camera" + std::to_string(ActiveCamera); 
	std::string tempS = "Cam" + std::to_string(ActiveCamera) + " Speed "; 
	char const* speed = tempS.c_str();
	char const* group = tempG.c_str();
	TwAddVarRW(m_bar, speed, TW_TYPE_FLOAT, &m_vListofCameras[ActiveCamera]->m_fSpeed, group); // +tostring(ActiveCamera);
	printf("~New Camera : %s\n", group);
	_DisableOtherCameras();

}

void Application::DestroyActiveCamera()
{
	if (this->m_vListofCameras.size() > 1)
	{
		// delete active camera
		//this->m_vListofCameras.
	}
}