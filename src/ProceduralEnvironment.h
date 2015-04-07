#ifndef _PROCEDURAL_ENVIRONMENT_H_
#define _PROCEDURAL_ENVIRONMENT_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"

class Building;

class ProceduralEnvironment : public Application
{
public:

	OpenGLData gridMesh;

	float *perlin_data;
	unsigned int perlin_1_texture;

	std::vector<Building*> buildings;



	MeshGroup* buildingBase;
	MeshGroup* bunnyObject;
	RenderTarget gBuffer;
	OpenGLData pointLight;
	unsigned int gBufferProgramID;
	unsigned int compositeProgramID;

	unsigned int perlinProgramID;
	
	unsigned int directionalLightProgramID;
	unsigned int pointLightProgramID;
	unsigned int spotLightProgramID;
	

	OpenGLData BuildGrid( glm::vec2 real_dims, glm::ivec2 dims );
	bool BuildPerlinTexture( unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset );



	virtual ~ProceduralEnvironment();

	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual void DebugDraw();

	// Render Code
	void RenderGeometry();
	void RenderLights();
	void DrawDirectionLight(const glm::vec3& a_direction, const glm::vec3& a_color);
	void DrawPointLight(glm::vec3 a_position, float a_radius, glm::vec3 a_color);
	void RenderComposite();
	
	void ReloadShaders();
	void LoadShaders();

};

#endif // _PROCEDURAL_ENVIRONMENT_H_