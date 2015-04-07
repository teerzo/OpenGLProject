#ifndef _PROCEDURAL_GENERATION_H_
#define _PROCEDURAL_GENERATION_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"

class ProceduralGeneration : public Application
{
public:

	OpenGLData gridMesh;

	float *perlin_data;
	

	unsigned int perlinProgramID;
	unsigned int perlinHeightTexture;
	unsigned int perlinGrassTexture;
	unsigned int perlinRockTexture;



	virtual ~ProceduralGeneration();

	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual void DebugDraw();
	void ReloadShaders();
	void LoadShaders();

	// Render Code
	void RenderGeometry();
	void RenderLights();
	void DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color);
	void DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color);
	void RenderComposite();


	OpenGLData BuildGrid(glm::vec2 real_dims, glm::ivec2 dims);
	bool BuildPerlinTexture(unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset);
	double perlin(double x, double y, double z);

	OpenGLData GeneratePlane();
	OpenGLData GenerateCube();
	void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes);

private:

};

#endif // _PROCEDURAL_GENERATION_H_