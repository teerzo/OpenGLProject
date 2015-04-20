#ifndef _PROCEDURAL_ENVIRONMENT_H_
#define _PROCEDURAL_ENVIRONMENT_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"
#include "Emitter.h"
#include "../deps/FBXLoader/FBXFile.h"

class Building;

struct TestObject {
	std::vector<OpenGLData> meshes;
	FBXSkeleton* skeleton;
	FBXAnimation* animation;

};

class ProceduralEnvironment : public Application
{
public:

	Emitter emitter;

	std::vector<glm::vec3>DEBUGNORMALS;
	std::vector<glm::vec3>DEBUGPOSITIONS;

	OpenGLData gridMesh;
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
	std::vector<MeshGroup*> soulspears;

	MeshGroup* buildingBase;
	MeshGroup* bunnyObject;

	unsigned int dirt_texture;
	unsigned int grass_texture;
	unsigned int lava_texture;
	unsigned int rock_texture;
	unsigned int soulspear_texture;

	glm::vec4 highest_position;
	glm::vec4 lowest_position;

	TwBar* tweakBarLighting;	
	// light tweak
	glm::vec3 tweak_light_direction;
	glm::vec3 tweak_light_color;
	float tweak_light_intensity;


	TwBar* tweakBarTerrain;
	// Perlin tweak
	glm::vec3 tweak_perlin_position;
	float tweak_perlin_size;
	float tweak_perlin_height;
	float tweak_perlin_octaves;
	float tweak_perlin_persistance;

	// Lava tweak
	glm::vec3 tweak_lava_direction;
	float tweak_lava_speed;
	float tweak_rock_height;
	float tweak_grass_height;
	float tweak_lava_height;
	float tweak_dirt_height;


	RenderTarget gBuffer;
	OpenGLData pointLight;
	unsigned int gBufferProgramID;
	unsigned int compositeProgramID;
	unsigned int particleProgramID;

	unsigned int perlinUpdateProgramID;
	unsigned int perlinDrawProgramID;
	
	unsigned int directionalLightProgramID;
	unsigned int pointLightProgramID;
	unsigned int spotLightProgramID;
	
	// Animation
	FBXFile* m_file;
	TestObject m_pyro;

	void GenerateGLMeshes( FBXFile* fbx, TestObject& object );
	void EvaluateSkeleton( FBXAnimation* anim, FBXSkeleton* skeleton, float timer );
	void UpdateBones( FBXSkeleton* skeleton );


	void DrawParticles();
	void DrawAnimation();

	OpenGLData BuildGrid(glm::vec2 real_dims, glm::ivec2 dims);
	OpenGLData BuildGridWithNormals(glm::vec2 real_dims, glm::ivec2 dims);
	
	void BuildVertexGrid( glm::vec2 real_dims, glm::ivec2 dims );
	bool BuildPerlinTexture( unsigned int* a_texture, const glm::ivec2 dims, const int octaves, const float persistance, const float offset );
	double Perlin(double x, double y, double z);

	virtual ~ProceduralEnvironment();

	virtual void SetApplicationDefaults();
	virtual bool ApplicationStartup();
	virtual void ApplicationShutdown();
	virtual void CheckInput();
	virtual bool Update();
	virtual void Draw();
	virtual void DebugDraw();
	void Reload();
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