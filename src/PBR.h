#ifndef _ADVANCED_NAVIGATION_H_
#define _ADVANCED_NAVIGATION_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"

class Building;

struct NavMeshNode
{
	glm::vec3 position;
	glm::vec3 vertices[3];
	NavMeshNode* edges[3];

	unsigned int flags;
	float edgeCosts[3];
};


class AdvancedNavigation : public Application
{
public:

	std::vector<NavMeshNode> nav_nodes;
	MeshGroup* buildingBase;
	MeshGroup* buildingNavMesh;

	RenderTarget gBuffer;
	OpenGLData pointLight;
	unsigned int gBufferProgramID;
	unsigned int directionalLightProgramID;
	unsigned int pointLightProgramID;
	unsigned int spotLightProgramID;
	unsigned int compositeProgramID;
	unsigned int sponzaProgramID;

	bool BuildNavMesh();






	virtual ~AdvancedNavigation();

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

#endif // _ADVANCED_NAVIGATION_H_