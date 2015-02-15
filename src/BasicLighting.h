#ifndef _BASIC_LIGHTING_H_
#define _BASIC_LIGHTING_H_

#include "Application.h"
#include "Vertex.h"
#include "tiny_obj_loader.h"

class Camera;
class FlyCamera;

class BasicLighting : public Application
{
private:

	// Lighting Code
	glm::vec3 eye_pos;
	float specular_power;
	glm::vec3 ambient_light;
	glm::vec3 light_dir;
	glm::vec3 light_color;
	glm::vec3 material_color;

	std::vector<OpenGLData> m_gl_data;
	OpenGLData m_gl_Quad;
	
public:
	virtual ~BasicLighting();

	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();

	// BasicLighting functions
	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes );
	//void createOpenGLBuffers(FBXFile* a_file);
	void cleanupOpenGLBuffers();
	//void cleanupOpenGLBuffers(FBXFile* a_file);
	void reloadShader();


	// Non BasicLighting functions
	virtual bool loadShaders();
	void loadTexture(const char* a_fileName);
	void generateQuad(float a_size);
	void generateGrid(unsigned int rows, unsigned int cols);
};

#endif // _BASIC_LIGHTING_H_