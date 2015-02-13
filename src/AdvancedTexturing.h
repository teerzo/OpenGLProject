#ifndef _ADVANCED_TEXTURING_H_
#define _ADVANCED_TEXTURING_H_

#include "Application.h"
#include "Vertex.h"
#include "tiny_obj_loader.h"

class Camera;
class FlyCamera;

//class FBXFile;
//class tinyobj;

struct OpenGLData
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

class AdvancedTexturing : public Application
{
private:
	VertexNormal test;
	OpenGLData m_quad;
	unsigned int m_diffuse_texture;
	unsigned int m_normal_texture;
	unsigned int m_specular_texture;

	glm::vec3 m_ambient_light;
	glm::vec3 m_light_dir;
	glm::vec3 m_light_color;
	float m_specular_power;

public:
	virtual ~AdvancedTexturing();

	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	// Advanced Texturing
	// diffuse, normal
	void loadTextures();



	// BasicLighting functions
	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes);
	//void createOpenGLBuffers(FBXFile* a_file);
	void cleanupOpenGLBuffers();
	//void cleanupOpenGLBuffers(FBXFile* a_file);
	void reloadShader();

	// Non BasicLighting functions
	virtual bool loadShaders();
	// diffuse only
	void loadTexture(const char* a_fileName);
	void generateQuad(float a_size);
	void generateGrid(unsigned int rows, unsigned int cols);
};

#endif // _BASIC_LIGHTING_H_