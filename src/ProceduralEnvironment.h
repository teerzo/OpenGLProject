#ifndef _PROCEDURAL_ENVIRONMENT_H_
#define _PROCEDURAL_ENVIRONMENT_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"

class Building;

class ProceduralEnvironment : public Application
{
public:

	//OpenGLData gridMesh[2];
	unsigned int active_buffer;
	unsigned int vao[2];
	unsigned int vbo[2]; 
	unsigned int grid_size;
	float last_draw_time;

	void CreateUpdateShader();
	void DrawPerlin();


	float *perlin_data;
	unsigned int perlin_1_texture;

	std::vector<Building*> buildings;



	MeshGroup* buildingBase;
	MeshGroup* bunnyObject;
	RenderTarget gBuffer;
	OpenGLData pointLight;
	unsigned int gBufferProgramID;
	unsigned int compositeProgramID;

	unsigned int perlinUpdateProgramID;
	unsigned int perlinDrawProgramID;
	
	unsigned int directionalLightProgramID;
	unsigned int pointLightProgramID;
	unsigned int spotLightProgramID;
	

	OpenGLData BuildGrid( glm::vec2 real_dims, glm::ivec2 dims );
	void BuildVertexGrid( glm::vec2 real_dims, glm::ivec2 dims );
	bool BuildPerlinTexture( unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset );



	virtual ~ProceduralEnvironment();

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
	
	

};

#endif // _PROCEDURAL_ENVIRONMENT_H_