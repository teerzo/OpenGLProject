#include "AdamUlbricht.h"

#include "gl_core_4_4.h"
#include <glfw3.h>
#include <cstdio>
#include "Gizmos.h"
#include "stb_image.h"


#include "Utility.h"
#include "Camera.h"

AdamUlbricht::~AdamUlbricht()
{

}

void AdamUlbricht::setDefaults()
{
	this->AppName = "AdamUlbricht";
	this->ScreenSize.Width = 1440;
	this->ScreenSize.Height = 900;
}

bool AdamUlbricht::startup()
{
	if (!Application::startup())
	{
		return false;
	}
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	Gizmos::create();
	glfwSetTime(0.0);

	std::vector <tinyobj::shape_t> shapes;
	std::vector <tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, "./models/AdamUlbricht/Grub.obj");
	
	if (err.size() != 0)
	{
		printf("Mesh load error %s\n", err);
		return false;
	}
	createOpenGLBuffers(shapes);
	loadShaders();

	loadDiffuseNormalSpecular("./textures/AdamUlbricht/Grub_tex.tga", "./textures/AdamUlbricht/Grub_norm.tga", "./textures/AdamUlbricht/Grub_spec.tga");

	eye_pos = glm::vec3(0);
	specular_power = 15.0f;
	ambient_light = glm::vec3(0.1f);
	light_dir = glm::vec3(0, -1.0f, 0);
	light_color = glm::vec3(0.6f, 0, 0);
	material_color = glm::vec3(1);

	return true;
}

void AdamUlbricht::shutdown()
{
	Application::shutdown();
}

bool AdamUlbricht::update()
{
	if (!Application::update())
	{
		return false;
	}
	//////////////////////////////////////
	//! Project Specific Update Code Here
	//////////////////////////////////////

	// reloadShader() hotkey
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		reloadShader();
	}


	///////////////////////
	//! End of Update Code
	///////////////////////


	return true;
}

void AdamUlbricht::draw()
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


	/*Cameratemp = m_vListofCameras[0]->getPosition();
	int cameraPos = glGetUniformLocation(m_ProgramID, "cameraPos");
	glUniform3f(cameraPos, Cameratemp.x, Cameratemp.y, Cameratemp.z);
	*/
	eye_pos = m_vListofCameras[ActiveCamera]->getPosition();
	int eye_pos_uniform = glGetUniformLocation(m_ProgramID, "eye_pos");
	glUniform3f(eye_pos_uniform, eye_pos.x, eye_pos.y, eye_pos.z);

	light_dir = m_vListofCameras[0]->getForward();
	int light_dir_uniform = glGetUniformLocation(m_ProgramID, "light_dir");
	glUniform3f(light_dir_uniform, light_dir.x, light_dir.y, light_dir.z);

	int specular_power_uniform = glGetUniformLocation(m_ProgramID, "specular_power");
	glUniform1f(specular_power_uniform, specular_power);

	int ambient_uniform = glGetUniformLocation(m_ProgramID, "ambient_light");
	glUniform3fv(ambient_uniform, 1, (float*)&ambient_light);

	int light_color_uniform = glGetUniformLocation(m_ProgramID, "light_color");
	glUniform3fv(light_color_uniform, 1, (float*)&light_color);

	int material_color_uniform = glGetUniformLocation(m_ProgramID, "material_color");
	glUniform3fv(material_color_uniform, 1, (float*)&material_color);
	
	//// OPENGL TEXTURE CALLS
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_grub.m_diffuse_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_grub.m_normal_texture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_grub.m_specular_texture);

	int diffuseLocation = glGetUniformLocation(m_ProgramID, "diffuse");
	glUniform1i(diffuseLocation, 0);
	int normalLocation = glGetUniformLocation(m_ProgramID, "normal");
	glUniform1i(normalLocation, 1);
	int specularLocation = glGetUniformLocation(m_ProgramID, "specular");
	glUniform1i(specularLocation, 2);

	for (unsigned int mesh_index = 0; mesh_index < m_grub.m_gl_data.size(); ++mesh_index)
	{
		glBindVertexArray(m_grub.m_gl_data[mesh_index].m_VAO);
		glDrawElements(GL_TRIANGLES, m_grub.m_gl_data[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}

	//////////////////////
	//! End of Draw Code 
	//////////////////////
	Application::draw();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void AdamUlbricht::createOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes)
{
	m_grub.m_gl_data.resize(a_shapes.size());

	for (unsigned int shape_index = 0; shape_index < a_shapes.size(); ++shape_index)
	{
		std::vector<float> vertex_data;

		unsigned int float_count = a_shapes[shape_index].mesh.positions.size();
		float_count += a_shapes[shape_index].mesh.normals.size();
		float_count += a_shapes[shape_index].mesh.texcoords.size();
		vertex_data.reserve(float_count);

		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.positions.begin(), a_shapes[shape_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.normals.begin(), a_shapes[shape_index].mesh.normals.end());
		vertex_data.insert(vertex_data.end(), a_shapes[shape_index].mesh.texcoords.begin(), a_shapes[shape_index].mesh.texcoords.end());

		m_grub.m_gl_data[shape_index].m_index_count = a_shapes[shape_index].mesh.indices.size();

		// generate ....
		glGenVertexArrays(1, &m_grub.m_gl_data[shape_index].m_VAO);
		glGenBuffers(1, &m_grub.m_gl_data[shape_index].m_VBO);
		glGenBuffers(1, &m_grub.m_gl_data[shape_index].m_IBO);

		glBindVertexArray(m_grub.m_gl_data[shape_index].m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_grub.m_gl_data[shape_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* float_count, vertex_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_grub.m_gl_data[shape_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_shapes[shape_index].mesh.indices.size() * sizeof(unsigned int), a_shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // UV
		glEnableVertexAttribArray(2); // Normal

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*a_shapes[shape_index].mesh.positions.size()));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*a_shapes[shape_index].mesh.texcoords.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void AdamUlbricht::cleanupOpenGLBuffers()
{
	glDeleteProgram(m_ProgramID);
	for (unsigned int i = 0; i < m_grub.m_gl_data.size(); ++i)
	{
		glDeleteVertexArrays(1, &m_grub.m_gl_data[i].m_VAO);
		glDeleteBuffers(1, &m_grub.m_gl_data[i].m_VBO);
		glDeleteBuffers(1, &m_grub.m_gl_data[i].m_IBO);

	}
}

void AdamUlbricht::reloadShader()
{
	glDeleteProgram(m_ProgramID);
	
	loadShaders();
	
}

bool AdamUlbricht::loadShaders()
{
	if (!LoadShader("./shaders/GrubVertex.glsl", "./shaders/GrubFragment.glsl", (GLuint*)&m_ProgramID))
	{
		return false;
	}
	return true;
}

void AdamUlbricht::loadTexture(const char* a_fileName)
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
	/// $$$ Change to take in object to place textures into aka diffuse, normal , spec , grub struct m_grub 
void AdamUlbricht::loadDiffuseNormalSpecular(const char* a_DiffuseFile, const char* a_NormalFile, const char* a_SpecularFile)
{
	int width, height, channels;

	unsigned char *data;

	data = stbi_load(a_DiffuseFile, &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_grub.m_diffuse_texture);
	glBindTexture(GL_TEXTURE_2D, m_grub.m_diffuse_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	data = stbi_load(a_NormalFile, &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_grub.m_normal_texture);
	glBindTexture(GL_TEXTURE_2D, m_grub.m_normal_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	data = stbi_load(a_SpecularFile, &width, &height, &channels, STBI_default);
	glGenTextures(1, &m_grub.m_specular_texture);
	glBindTexture(GL_TEXTURE_2D, m_grub.m_specular_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
}