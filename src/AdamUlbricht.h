#ifndef _ADAM_ULBRICHT_H_
#define _ADAM_ULBRICHT_H_

#include "Application.h"
#include "Vertex.h"
#include "tiny_obj_loader.h"

struct Grub
{
	std::vector<OpenGLData> m_gl_data;
	unsigned int m_diffuse_texture;
	unsigned int m_normal_texture;
	unsigned int m_specular_texture;
};

class AdamUlbricht : public Application
{
private:
	// Base Variables
	
	// Shader Uniforms
	glm::vec3 eye_pos;
	float specular_power;
	glm::vec3 ambient_light;
	glm::vec3 light_dir;
	glm::vec3 light_color;
	glm::vec3 material_color;

	// Project Specific
	Grub m_grub;
	
	

public:
	virtual ~AdamUlbricht();

	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes);
	void cleanupOpenGLBuffers();
	void reloadShader();

	virtual bool loadShaders();
	void loadTexture(const char* a_fileName);
	void loadDiffuseNormalSpecular(const char* a_DiffuseFile, const char* a_NormalFile, const char* a_SpecularFile);
};

#endif // _ADAM_ULBRICHT_H_