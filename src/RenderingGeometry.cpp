#include "RenderingGeometry.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Gizmos.h"

#include "Utility.h"

#include "Camera.h"

RenderingGeometry::~RenderingGeometry()
{

}


void RenderingGeometry::setDefaults()
{
	this->AppName = "Rendering Geometry";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;


}


bool RenderingGeometry::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	generateShader();
	generateGrid(10, 10);

	LoadShader("./data/shaders/BasicVertex.glsl", "./ data / shaders / BasicFragment.glsl", (GLuint*)m_ProgramID);

	//ProjectionMatrix = glm::perspective(glm::radians(80.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	this->m_vListofCameras.push_back(new FlyCamera());
	this->m_vListofCameras.push_back(new FlyCamera());

	//this->Camera = new Camera();
	//this->flyCamera = new FlyCamera();

	m_vListofCameras[0]->m_bIsSelected = true;
	m_vListofCameras[1]->m_bIsSelected = false;

	//glClearColor()


	return true;
}

bool RenderingGeometry::loadShaders()
{
	
	return true;
}

void RenderingGeometry::shutdown()
{
	Gizmos::destroy();
}

bool RenderingGeometry::update()
{
	if (!Application::update())
	{
		return false;
	}
	// Cool code here please
	m_fTimer = (float)glfwGetTime();
	float deltaTime = m_fTimer - m_fPreviousTime; // prev of last frame
	m_fPreviousTime = m_fTimer;
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::clear();

	for (int i = 0; i <= 20; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, -10), glm::vec3(-10 + i, 0, 10), i == 10 ? color.White : color.Gray);
		Gizmos::addLine(glm::vec3(-10, 0, -10 + i), glm::vec3(10, 0, -10 + i), i == 10 ? color.White : color.Gray);
	}

	for (unsigned i = 0; i < m_vListofCameras.size(); ++i)
	{
		m_vListofCameras[i]->update(deltaTime);
		Gizmos::addAABB(m_vListofCameras[i]->getPosition(), glm::vec3(1.2, 1.2, 1.2), color.Red);
		Gizmos::addLine(m_vListofCameras[i]->getPosition(), m_vListofCameras[i]->getPosition() + (m_vListofCameras[i]->getForward() * -2), color.Blue);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_vListofCameras[0]->m_bIsSelected = true;
		m_vListofCameras[1]->m_bIsSelected = false;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_vListofCameras[0]->m_bIsSelected = false;
		m_vListofCameras[1]->m_bIsSelected = true;
	}

	Gizmos::addAABB(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), color.Blue);

	

	return true;
}

void RenderingGeometry::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ProgramID);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[0]->getProjectionView());
	}
	int shaderTimer = glGetUniformLocation(m_ProgramID, "timer");
	glUniform1f(shaderTimer, m_fTimer);

	glm::vec3 temp = m_vListofCameras[0]->getPosition();
	int cameraPos = glGetUniformLocation(m_ProgramID, "cameraPos");
	glUniform3f(cameraPos, temp.x, temp.y, temp.z);

	// missing code here $$$
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(m_VAO);
	
	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	if (m_vListofCameras[0]->m_bIsSelected)
	{
		Gizmos::draw(m_vListofCameras[0]->getProjectionView());
	}
	else
	{
		Gizmos::draw(m_vListofCameras[1]->getProjectionView());
	}

	
	Application::draw();

	glfwSwapBuffers(this->window);
	glfwPollEvents();
	
}


void RenderingGeometry::generateGrid(unsigned rows, unsigned cols)
{
	Vertex* vertex_array = new Vertex[(rows + 1) * (cols + 1)];
	for (unsigned r = 0; r < rows + 1; ++r)
	{
		for (unsigned c = 0; c < cols + 1; ++c)
		{
			vertex_array[c + r * (cols + 1)].Position = glm::vec4((float)c, 0, (float)r, 1);
			vertex_array[c + r * (cols + 1)].Color = glm::vec4((float)c / (cols + 1), 0, (float)r / (rows + 1), 1);
			//vertex_array[c + r * (cols + 1)].Normal = glm::vec4(1);
			//vertex_array[c + r * (cols + 1)].UV = glm::vec2(1);
		}
	}
	m_index_count = rows*cols * 6;
	unsigned *index_array = new unsigned[rows*cols * 6];
	int index_Location = 0;
	for (unsigned r = 0; r < rows; ++r)
	{
		for (unsigned c = 0; c < cols; ++c)
		{
			index_array[index_Location + 0] = c + r * (cols+1);
			index_array[index_Location + 1] = c + (r + 1) * (cols+1 );
			index_array[index_Location + 2] = (c + 1) + r * (cols +1);

			index_array[index_Location + 3] = (c + 1) + r * (cols+1 );
			index_array[index_Location + 4] = c + (r + 1) * (cols+1 );
			index_array[index_Location + 5] = (c + 1) + (r + 1) * (cols+1 );

			index_Location += 6;
		}
	}

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (cols + 1)*(rows + 1)*sizeof(Vertex), vertex_array, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color
	//glEnableVertexAttribArray(2); // Normal
	//glEnableVertexAttribArray(3); // UV


	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4) * 2);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2 ) + (sizeof(glm::vec2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(unsigned int), index_array, GL_STATIC_DRAW);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertex_array;
	delete[] index_array;
}

void RenderingGeometry::generateShader()
{
	


}