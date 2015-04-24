#ifndef _KM_APP_CHECKERS_H_
#define _KM_APP_CHECKERS_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"

class kmAppCheckers : public Application
{
public:

	TwBar* tweakBarLighting;
	// light tweak
	glm::vec3 tweak_light_direction;
	glm::vec3 tweak_light_color;
	float tweak_light_intensity;


	RenderTarget* deferred_render;
	OpenGLData pointLight;
	unsigned int gBufferProgramID;
	unsigned int compositeProgramID;
	unsigned int particleProgramID;

	unsigned int perlinUpdateProgramID;
	unsigned int perlinDrawProgramID;

	unsigned int directionalLightProgramID;
	unsigned int pointLightProgramID;
	unsigned int spotLightProgramID;


	virtual ~kmAppCheckers();

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