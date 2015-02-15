#include "AdvancedTexturing.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Gizmos.h"
#include "stb_image.h"


#include "Utility.h"
#include "Camera.h"


AdvancedTexturing::~AdvancedTexturing()
{

}


void AdvancedTexturing::setDefaults()
{
	this->AppName = "Basic Lighting";
	this->ScreenSize.Width = 1280;
	this->ScreenSize.Height = 720;


}


bool AdvancedTexturing::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	//m_fbx_file = new FBXFile();
	//m_fbx_file->load("models/stanford/Bunny.fbx");

	//std::vector <tinyobj::shape_t> shapes;
	//std::vector <tinyobj::material_t> materials;

//	std::string err = tinyobj::LoadObj(shapes, materials, "models/stanford/bunny.obj");
	//printf("Mesh load error %s\n", err);
	//if (err.size() != 0)
//	{
	//	return false;
//	}

	generateQuad(10.0f);
	loadTextures();
	LoadShader("shaders/NormalMapVertex.glsl", "shaders/NormalMapFragment.glsl", (GLuint*)&m_ProgramID);

	//createOpenGLBuffers(m_fbx_file);
	//createOpenGLBuffers(shapes);
	//LoadShader("shaders/Lighting_vertex.glsl", "shaders/Lighting_fragment.glsl", (GLuint*)&m_ProgramID);

	//generateGrid(10, 10);

	//loadTexture("textures/crate.png");
	//generateQuad(5.0f);
	//LoadShader("shaders/TexturedVertex.glsl", "shaders/TexturedFragment.glsl", (GLuint*)&m_ProgramID);

	// lighting code
	//eye_pos = glm::vec3(0);
	m_specular_power = 15.0f;
	m_ambient_light = glm::vec3(0.1f);
	//m_light_dir = (glm::rotation(dt, glm::vec3(0, -1.0f, 0)) * glm::vec4(m_light_dir, 0).xyz;
	m_light_color = glm::vec3(0.6f, 0, 0);
	//material_color = glm::vec3(1);

	
		

	return true;
}


void AdvancedTexturing::shutdown()
{
	//cleanupOpenGLBuffers(m_fbx_file);
	cleanupOpenGLBuffers();
	Application::shutdown();
}

bool AdvancedTexturing::update()
{
	if (!Application::update())
	{
		return false;
	}
	// Cool code here please





	// reloadShader() hotkey
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		reloadShader();
	}


	return true;
}

void AdvancedTexturing::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);

	int proj_view_handle = glGetUniformLocation(m_ProgramID, "ProjectionView");
	if (proj_view_handle >= 0)
	{
		glUniformMatrix4fv(proj_view_handle, 1, GL_FALSE, (float*)&m_vListofCameras[ActiveCamera]->getProjectionView());

	}
	////////////////////////////////
	//! Advanced Texturing Drawcode 
	////////////////////////////////

	/*for (unsigned int mesh_index = 0; mesh_index < m_gl_data.size(); ++mesh_index)
	{
		glBindVertexArray(m_gl_data[mesh_index].m_VAO);
		glDrawElements(GL_TRIANGLES, m_gl_data[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}*/
	/*for (unsigned int mesh_index = 0; mesh_index < m_fbx_file->getMeshCount(); ++mesh_index)
	{
	FBXMeshNode* mesh = m_fbx_file->getMeshByIndex(mesh_index);

	unsigned int* gl_data = (unsigned int*)mesh->m_userData;

	glBindVertexArray(gl_data[0]);
	glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);

	}*/

	int shaderTimer = glGetUniformLocation(m_ProgramID, "timer");
	glUniform1f(shaderTimer, m_fTimer);


	/*Cameratemp = m_vListofCameras[0]->getPosition();
	int cameraPos = glGetUniformLocation(m_ProgramID, "cameraPos");
	glUniform3f(cameraPos, Cameratemp.x, Cameratemp.y, Cameratemp.z);
	*/
	//eye_pos = m_vListofCameras[ActiveCamera]->getPosition();
	//int eye_pos_uniform = glGetUniformLocation(m_ProgramID, "eye_pos");
	//glUniform3f(eye_pos_uniform, eye_pos.x, eye_pos.y, eye_pos.z);

	m_light_dir = m_vListofCameras[2]->getForward();
	int light_dir_uniform = glGetUniformLocation(m_ProgramID, "light_dir");
	glUniform3f(light_dir_uniform, m_light_dir.x, m_light_dir.y, m_light_dir.z);

	int specular_power_uniform = glGetUniformLocation(m_ProgramID, "specular_power");
	glUniform1f(specular_power_uniform, m_specular_power);

	int ambient_uniform = glGetUniformLocation(m_ProgramID, "ambient_light");
	glUniform3fv(ambient_uniform, 1, (float*)&m_ambient_light);

	int light_color_uniform = glGetUniformLocation(m_ProgramID, "light_color");
	glUniform3fv(light_color_uniform, 1, (float*)&m_light_color);

	//int material_color_uniform = glGetUniformLocation(m_ProgramID, "material_color");
	//glUniform3fv(material_color_uniform, 1, (float*)&material_color);



	//// OPENGL TEXTURE CALLS
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	int diffuseLocation = glGetUniformLocation(m_ProgramID, "diffuse");
	glUniform1i(diffuseLocation, 0);
	int normalLocation = glGetUniformLocation(m_ProgramID, "normal");
	glUniform1i(normalLocation, 1);
	int specularLocation = glGetUniformLocation(m_ProgramID, "specular");
	glUniform1i(specularLocation, 2);

	//
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(m_VAO);
	//glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
	
	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();

}

void AdvancedTexturing::loadTextures()
{
	int width, height, channels;

	unsigned char *data;

	data = stbi_load("textures/rock_diffuse.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_diffuse_texture);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	data = stbi_load("textures/rock_normal.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_normal_texture);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	data = stbi_load("textures/rock_specular.tga", &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_specular_texture);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}


void AdvancedTexturing::createOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes)
{
	//m_gl_data.resize(a_shapes.size());

	//for (unsigned int shape_index = 0; shape_index < a_shapes.size(); ++shape_index)
	//{
	//	std::vector<float> vertex_data;

	//	unsigned int float_count = a_shapes[shape_index].mesh.positions.size();
	//	float_count += a_shapes[shape_index].mesh.normals.size();
	//	vertex_data.reserve(float_count);

	//	vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.positions.begin(), a_shapes[shape_index].mesh.positions.end());
	//	vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.normals.begin(), a_shapes[shape_index].mesh.normals.end());

	//	m_gl_data[shape_index].m_index_count = a_shapes[shape_index].mesh.indices.size();

	//	// generate ....
	//	glGenVertexArrays(1, &m_gl_data[shape_index].m_VAO);
	//	glGenBuffers(1, &m_gl_data[shape_index].m_VBO);
	//	glGenBuffers(1, &m_gl_data[shape_index].m_IBO);

	//	glBindVertexArray(m_gl_data[shape_index].m_VAO);

	//	glBindBuffer(GL_ARRAY_BUFFER, m_gl_data[shape_index].m_VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* float_count, vertex_data.data(), GL_STATIC_DRAW);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_data[shape_index].m_IBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_shapes[shape_index].mesh.indices.size() * sizeof(unsigned int), a_shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);

	//	glEnableVertexAttribArray(0); // Position
	//	glEnableVertexAttribArray(1); // Normal data

	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void*)(sizeof(float)*a_shapes[shape_index].mesh.positions.size()));

	//	glBindVertexArray(0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}
}

//void AdvancedTexturing::createOpenGLBuffers(FBXFile* a_file)
//{
//	for (unsigned int mesh_index = 0; mesh_index < a_file->getMeshCount(); ++mesh_index)
//	{
//		FBXMeshNode* mesh = a_file->getMeshByIndex(mesh_index);
//		// contains VBO, IBO and VAO
//		unsigned int* gl_data = new unsigned int[3];
//		
//		// generate ....
//		glGenVertexArrays(1, &gl_data[0]);
//		glGenBuffers(2, &gl_data[1]);
//
//		glBindVertexArray(gl_data[0]);
//		
//		glBindBuffer(GL_ARRAY_BUFFER, gl_data[1]);
//		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data[2]);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0); // Position
//		glEnableVertexAttribArray(1); // Normal data
//
//		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
//		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)(FBXVertex::NormalOffset));
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//		mesh->m_userData = gl_data;
//	}
//
//
//
//}

void AdvancedTexturing::cleanupOpenGLBuffers()
{
	/*glDeleteProgram(m_ProgramID);
	for (unsigned int i = 0; i < m_gl_data.size(); ++i)
	{
		glDeleteVertexArrays(1, &m_gl_data[i].m_VAO);
		glDeleteBuffers(1, &m_gl_data[i].m_VBO);
		glDeleteBuffers(1, &m_gl_data[i].m_IBO);

	}*/
}
//void AdvancedTexturing::cleanupOpenGLBuffers(FBXFile* a_file)
//{
//	for (unsigned int mesh_index = 0; mesh_index < a_file->getMeshCount(); ++mesh_index)
//	{
//		FBXMeshNode* mesh = a_file->getMeshByIndex(mesh_index);
//
//		unsigned int* gl_data = (unsigned int*)mesh->m_userData;
//
//		glDeleteVertexArrays(1, &gl_data[0]);
//		glDeleteBuffers(2, &gl_data[1]);
//
//		delete[] gl_data;
//	}
//}

void AdvancedTexturing::reloadShader()
{
	glDeleteProgram(m_ProgramID);
	LoadShader("shaders/Lighting_vertex.glsl", "shaders/Lighting_fragment.glsl", (GLuint*)&m_ProgramID);
}


bool AdvancedTexturing::loadShaders()
{

	return true;
}

void AdvancedTexturing::loadTexture(const char* a_fileName)
{
	/*int width;
	int height;

	int channels;

	unsigned char* data = stbi_load(a_fileName, &width, &height, &channels, STBI_default);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);*/
}




void AdvancedTexturing::generateQuad(float a_size)
{
	VertexNormal vertex_data[4];
	
	
	vertex_data[0].Position = glm::vec4(-a_size, 1, -a_size, 1);
	vertex_data[1].Position = glm::vec4(-a_size, 1, a_size, 1);
	vertex_data[2].Position = glm::vec4(a_size, 1, a_size, 1);
	vertex_data[3].Position = glm::vec4(a_size, 1, -a_size, 1);

	vertex_data[0].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[1].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[2].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[3].Normal = glm::vec4(0, 1, 0, 0);

	vertex_data[0].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[1].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[2].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[3].Tangent = glm::vec4(1, 0, 0, 0);

	vertex_data[0].UV = glm::vec2(0, 0);
	vertex_data[1].UV = glm::vec2(0, 1);
	vertex_data[2].UV = glm::vec2(1, 1);
	vertex_data[3].UV = glm::vec2(1, 0);
	
	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };
	m_quad.m_index_count = 6;

	glGenVertexArrays(1, &m_quad.m_VAO);

	glGenBuffers(1, &m_quad.m_VBO);
	glGenBuffers(1, &m_quad.m_IBO);
	glBindVertexArray(m_quad.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_quad.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // Tangent
	glEnableVertexAttribArray(3); // UV

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4)* 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void AdvancedTexturing::generateGrid(unsigned rows, unsigned cols)
{
	//Vertex* vertex_array = new Vertex[(rows + 1) * (cols + 1)];
	//for (unsigned r = 0; r < rows + 1; ++r)
	//{
	//	for (unsigned c = 0; c < cols + 1; ++c)
	//	{
	//		vertex_array[c + r * (cols + 1)].Position = glm::vec4((float)c, 0, (float)r, 1);
	//		vertex_array[c + r * (cols + 1)].Color = glm::vec4((float)c / (cols + 1), 0, (float)r / (rows + 1), 1);
	//		vertex_array[c + r * (cols + 1)].UV = glm::vec2(1);
	//		//vertex_array[c + r * (cols + 1)].Normal = glm::vec4(1);
	//	}
	//}
	//m_index_count = rows*cols * 6;
	//unsigned *index_array = new unsigned[rows*cols * 6];
	//int index_Location = 0;
	//for (unsigned r = 0; r < rows; ++r)
	//{
	//	for (unsigned c = 0; c < cols; ++c)
	//	{
	//		index_array[index_Location + 0] = c + r * (cols + 1);
	//		index_array[index_Location + 1] = c + (r + 1) * (cols + 1);
	//		index_array[index_Location + 2] = (c + 1) + r * (cols + 1);

	//		index_array[index_Location + 3] = (c + 1) + r * (cols + 1);
	//		index_array[index_Location + 4] = c + (r + 1) * (cols + 1);
	//		index_array[index_Location + 5] = (c + 1) + (r + 1) * (cols + 1);

	//		index_Location += 6;
	//	}
	//}

	//glGenBuffers(1, &m_VBO);
	//glGenBuffers(1, &m_IBO);
	//glGenVertexArrays(1, &m_VAO);

	//glBindVertexArray(m_VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBufferData(GL_ARRAY_BUFFER, (cols + 1)*(rows + 1)*sizeof(Vertex), vertex_array, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0); // Position
	//glEnableVertexAttribArray(1); // Color
	////glEnableVertexAttribArray(2); // Normal
	////glEnableVertexAttribArray(3); // UV


	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
	////glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4) * 2);
	////glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2 ) + (sizeof(glm::vec2);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(unsigned int), index_array, GL_STATIC_DRAW);
	//glBindVertexArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//delete[] vertex_array;
	//delete[] index_array;
}

