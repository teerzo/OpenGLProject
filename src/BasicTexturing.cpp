#include "BasicTexturing.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Gizmos.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Utility.h"
#include "Camera.h"

BasicTexturing::~BasicTexturing()
{

}


void BasicTexturing::setDefaults()
{
	this->AppName = "Basic Texturing";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;


}


bool BasicTexturing::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	glfwSetTime(0.0);


	//generateGrid(10, 10);

	loadTexture("textures/crate.png");
	generateQuad(5.0f);
	
	loadShaders();

	return true;
}

bool BasicTexturing::loadShaders()
{
	if (!LoadShader("shaders/TexturedVertex.glsl", "shaders/TexturedFragment.glsl", (GLuint*)&m_ProgramID))
	{
		return false;
	}
	return true;
}

void BasicTexturing::loadTexture(const char* a_fileName)
{
	int width;
	int height;

	int channels;

	unsigned char* data = stbi_load(a_fileName, &width, &height, &channels, STBI_default);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

void BasicTexturing::shutdown()
{
	Gizmos::destroy();
}

bool BasicTexturing::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////
	
	

	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void BasicTexturing::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());
	
	}
	///////////////////////////////////
	//! Project Specific Drawcode Here 
	///////////////////////////////////
	
	int shaderTimer = glGetUniformLocation(m_ProgramID, "timer");
	glUniform1f(shaderTimer, m_fTimer);
	
	glm::vec3 Cameratemp = m_vListofCameras[0]->getPosition();
	int cameraPos = glGetUniformLocation(m_ProgramID, "cameraPos");
	glUniform3f(cameraPos, Cameratemp.x, Cameratemp.y, Cameratemp.z);

	// OPENGL TEXTURE CALLS
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	int diffuseLocation = glGetUniformLocation(m_ProgramID, "diffuse");
	glUniform1i(diffuseLocation, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(m_gl_data.m_VAO);
	glDrawElements(GL_TRIANGLES, m_gl_data.m_index_count, GL_UNSIGNED_INT, 0);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void BasicTexturing::generateQuad(float a_size)
{
	Vertex vertex_data[4];
	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };
	m_gl_data.m_index_count = 6;
	vertex_data[0].Position = glm::vec4(-a_size, 1, -a_size, 1);
	vertex_data[1].Position = glm::vec4(-a_size, 1, a_size, 1);
	vertex_data[2].Position = glm::vec4(a_size, 1, a_size, 1);
	vertex_data[3].Position = glm::vec4(a_size, 1, -a_size, 1);

	vertex_data[0].Color = glm::vec4(color.Red);
	vertex_data[1].Color = glm::vec4(color.Green);
	vertex_data[2].Color = glm::vec4(color.Blue);
	vertex_data[3].Color = glm::vec4(color.Gray);

	vertex_data[0].UV = glm::vec2(0, 0);
	vertex_data[1].UV = glm::vec2(0, 1);
	vertex_data[2].UV = glm::vec2(1, 1);
	vertex_data[3].UV = glm::vec2(1, 0);


	glGenVertexArrays(1, &m_gl_data.m_VAO);

	glGenBuffers(1, &m_gl_data.m_VBO);
	glGenBuffers(1, &m_gl_data.m_IBO);
	glBindVertexArray(m_gl_data.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_gl_data.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* 4, vertex_data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_data.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color
	glEnableVertexAttribArray(2); // UV

	int uvoffset = sizeof(glm::vec4) + sizeof(glm::vec4);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)uvoffset);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void BasicTexturing::generateGrid(unsigned rows, unsigned cols)
{
	Vertex* vertex_array = new Vertex[(rows + 1) * (cols + 1)];
	for (unsigned r = 0; r < rows + 1; ++r)
	{
		for (unsigned c = 0; c < cols + 1; ++c)
		{
			vertex_array[c + r * (cols + 1)].Position = glm::vec4((float)c, 0, (float)r, 1);
			vertex_array[c + r * (cols + 1)].Color = glm::vec4((float)c / (cols + 1), 0, (float)r / (rows + 1), 1);
			vertex_array[c + r * (cols + 1)].UV = glm::vec2(1);
			//vertex_array[c + r * (cols + 1)].Normal = glm::vec4(1);
		}
	}
	m_gl_data.m_index_count = rows*cols * 6;
	unsigned *index_array = new unsigned[rows*cols * 6];
	int index_Location = 0;
	for (unsigned r = 0; r < rows; ++r)
	{
		for (unsigned c = 0; c < cols; ++c)
		{
			index_array[index_Location + 0] = c + r * (cols + 1);
			index_array[index_Location + 1] = c + (r + 1) * (cols + 1);
			index_array[index_Location + 2] = (c + 1) + r * (cols + 1);

			index_array[index_Location + 3] = (c + 1) + r * (cols + 1);
			index_array[index_Location + 4] = c + (r + 1) * (cols + 1);
			index_array[index_Location + 5] = (c + 1) + (r + 1) * (cols + 1);

			index_Location += 6;
		}
	}

	glGenBuffers(1, &m_gl_data.m_VBO);
	glGenBuffers(1, &m_gl_data.m_IBO);
	glGenVertexArrays(1, &m_gl_data.m_VAO);

	glBindVertexArray(m_gl_data.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_gl_data.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (cols + 1)*(rows + 1)*sizeof(Vertex), vertex_array, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color
	//glEnableVertexAttribArray(2); // Normal
	//glEnableVertexAttribArray(3); // UV


	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4) * 2);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2 ) + (sizeof(glm::vec2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_data.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_gl_data.m_index_count * sizeof(unsigned int), index_array, GL_STATIC_DRAW);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertex_array;
	delete[] index_array;
}

