#include "ProceduralGeneration.h"

//#include "gl_core_4_4.h"
#include <glfw3.h>
//#include <cstdio>

#include "Gizmos.h"
#include "Camera.h"



ProceduralGeneration::~ProceduralGeneration()
{

}

void ProceduralGeneration::SetApplicationDefaults()
{
	this->applicationName = "ProceduralGeneration";
	this->screenSize.Width = 1280;
	this->screenSize.Height = 720;
}

bool ProceduralGeneration::ApplicationStartup()
{
	if (!Application::ApplicationStartup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	gridMesh = BuildGrid(glm::vec2(128, 128), glm::ivec2(128, 128));
	BuildPerlinTexture(&perlinHeightTexture,glm::ivec2(128, 128), 8, 0.3f, (float)0.0f);
	BuildPerlinTexture(&perlinGrassTexture,	glm::ivec2(128, 128), 8, 0.3f, (float)1.0f);
	BuildPerlinTexture(&perlinRockTexture,	glm::ivec2(128, 128), 8, 0.3f, (float)2.0f);

	// Load Plane
	//glPlane = GeneratePlane();
	// Load Mesh
	std::vector <tinyobj::shape_t> shapes;
	std::vector <tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, "stanford/bunny.obj");
	if (err.size() != 0)
	{
		printf("Mesh load error %s\n", err);

		return false;
	}

	//createOpenGLBuffers(m_fbx_file);
	CreateOpenGLBuffers(shapes);
	// Load Shaders
	LoadShaders();


	return true;
}

void ProceduralGeneration::ApplicationShutdown()
{
	Application::ApplicationShutdown();
}

bool ProceduralGeneration::Update()
{
	CheckInput();
	if (!Application::Update())
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

void ProceduralGeneration::Draw()
{
	//glCullFace(GL_BACK);

	//glEnable(GL_DEPTH_TEST);
	RenderGeometry();
	//glDisable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	RenderLights();
	//glDisable(GL_BLEND);

	RenderComposite();

	//////////////////////
	//! End of Draw Code 
	//////////////////////
	DebugDraw();
	Application::Draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void ProceduralGeneration::RenderGeometry()
{
	glViewport(0, 0, 1280, 720);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(perlinProgramID);

	int uniform_projection_view = glGetUniformLocation(perlinProgramID, "ProjectionView");
	glUniformMatrix4fv(uniform_projection_view, 1, GL_FALSE, (float*)&cameraVector[currentCamera]->GetProjectionView());

	int uniform_perlin_height_texture = glGetUniformLocation(perlinProgramID, "perlin_height_texture");
	glUniform1i(uniform_perlin_height_texture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, perlinHeightTexture);

	int uniform_perlin_grass_texture = glGetUniformLocation(perlinProgramID, "perlin_grass_texture");
	glUniform1i(uniform_perlin_grass_texture, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, perlinGrassTexture);

	int uniform_perlin_rock_texture = glGetUniformLocation(perlinProgramID, "perlin_rock_texture");
	glUniform1i(uniform_perlin_rock_texture, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, perlinRockTexture);


	glBindVertexArray(gridMesh.m_VAO);
	glDrawElements(GL_TRIANGLES, gridMesh.m_index_count, GL_UNSIGNED_INT, 0);
}

void ProceduralGeneration::RenderLights()
{


}
void ProceduralGeneration::DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color)
{

}

void ProceduralGeneration::DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color)
{

}

void ProceduralGeneration::RenderComposite()
{

}

void ProceduralGeneration::DebugDraw()
{
	Application::DebugDraw();
}

void ProceduralGeneration::CheckInput()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShaders();
	}
	Application::CheckInput();
}

OpenGLData ProceduralGeneration::GeneratePlane()
{
	OpenGLData temp;
	temp.m_index_count = 6;
	float vertexData[] = {
		-5, 0, -1, 1, 0, 0,
		5, 0, -1, 1, 1, 0,
		5, 10, -1, 1, 1, 1,
		-5, 10, -1, 1, 0, 1,
	};
	unsigned int index_data[] = {
		0, 1, 2,
		0, 2, 3,
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* temp.m_index_count * 4, vertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* temp.m_index_count, index_data, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position 
	glEnableVertexAttribArray(1); // UV
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, 0); // Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* temp.m_index_count, ((char*)0) + 16); // UV

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return temp;
}

void ProceduralGeneration::CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes)
{

}

void ProceduralGeneration::LoadShaders()
{
	LoadShader((GLuint*)&perlinProgramID, "perlin_vertex.glsl", "perlin_fragment.glsl", nullptr);

}

void ProceduralGeneration::ReloadShaders()
{
	printf("reloaded Shaders\n");
	glfwSetTime(0.0);
	currentGameTime = 0.0f;
	glDeleteProgram(perlinProgramID);
	LoadShaders();
}
OpenGLData ProceduralGeneration::GenerateCube()
{
	// Inverted Cube
	OpenGLData temp;
	temp.m_index_count = 36;

	float cubeVertexData[] = {
		-1, -1, 1, 1,
		1, -1, 1, 1,
		1, -1, -1, 1,
		-1, -1, -1, 1,
		-1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, -1, 1,
		-1, 1, -1, 1,
	};
	unsigned int cubeindex_data[] = {
		4, 5, 0,
		5, 1, 0,
		5, 6, 1,
		6, 2, 1,
		6, 7, 2,
		7, 3, 2,
		7, 4, 3,
		4, 0, 3,
		7, 6, 4,
		6, 5, 4,
		0, 1, 3,
		1, 2, 3
	};

	// Bind Vertex Data
	glGenVertexArrays(1, &temp.m_VAO);
	glBindVertexArray(temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

	// Bind Indices
	glGenBuffers(1, &temp.m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeindex_data), cubeindex_data, GL_STATIC_DRAW);

	// Enable Shader Code
	glEnableVertexAttribArray(0); // Position  
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 4, 0); // Position

	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return temp;
}


OpenGLData ProceduralGeneration::BuildGrid(glm::vec2 real_dims, glm::ivec2 dims)
{
	// Allocate vertex data
	OpenGLData temp;
	unsigned int vertex_count = (dims.x + 1) * (dims.y + 1);
	VertexUV* vertex_data = new VertexUV[vertex_count];

	unsigned int index_count = dims.x * dims.y * 6;
	unsigned int* index_data = new unsigned int[index_count];

	float curr_y = -real_dims.y / 2.0f;
	for ( int y = 0; y < dims.y + 1; ++y)
	{
		float curr_x = -real_dims.x / 2.0f;
		for ( int x = 0; x < dims.x + 1; ++x)
		{
			vertex_data[y * (dims.x + 1) + x].Position = glm::vec4(curr_x, 0, curr_y, 1);
			vertex_data[y * (dims.x + 1) + x].UV = glm::vec2((float)x / (float)dims.x, (float)y / (float)dims.y);
			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}

	int curr_index = 0;
	for (int y = 0; y < dims.y; ++y)
	{
		for (int x = 0; x < dims.x; ++x)
		{
			index_data[curr_index++] = y * (dims.x + 1) + x;
			index_data[curr_index++] = (y+ 1) * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + x;
		}
	}

	temp.m_index_count = index_count;
	//create vertexarrayobecject, buffers, etc
	glGenVertexArrays(1, &temp.m_VAO);
	glGenBuffers(1, &temp.m_VBO);
	glGenBuffers(1, &temp.m_IBO);

	//bind and fill buffers	
	glBindVertexArray(temp.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, temp.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexUV)* vertex_count, vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* index_count, index_data, GL_STATIC_DRAW);
		
	// tell opengl about our vertex structure
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexUV), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)sizeof( glm::vec4));

	// unbind stuff
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] index_data;
	delete[] vertex_data;
	
	return temp;
}

bool ProceduralGeneration::BuildPerlinTexture( unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset)
{
	bool result = true;
	// set scale
	float scale = (1.0f / dims.x) * 3.0f;

	// Allocate memory for perlin data
	perlin_data = new float[dims.x * dims.y];

	for (int y = 0; y < dims.y; ++y)
	{
		for (int x = 0; x < dims.x; ++x)
		{
			float amplitude = 1.0f;
			float frequency = 1.0f;
			perlin_data[y * dims.x + x] = 0;

			for (int i = 0; i < octaves; ++i)
			{
				float perlin_sample = glm::perlin(glm::vec2(x + offset, y + offset) * scale * frequency) * 0.5f + 0.5f;
				perlin_sample *= amplitude;
				perlin_data[y * dims.x + x] += perlin_sample;

				amplitude *= persistance;
				frequency *= 2.0f;
			}
		}
	}

	// generate opengl texture handles
	glGenTextures(1, a_texture);
	glBindTexture(GL_TEXTURE_2D, *a_texture);

	// pass the perlin data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims.x, dims.y, 0, GL_RED, GL_FLOAT, perlin_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return result;
}

