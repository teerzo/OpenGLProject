#ifndef _DEFERREDRENDERING_H_
#define _DEFERREDRENDERING_H_

#include "Application.h"
#include "RenderTarget.h"
#include "tiny_obj_loader.h"

class DeferredRendering : public Application
{
public:
	// Load Mesh

	MeshGroup* bunnyObject;
	//std::vector<OpenGLData> glData;
	//OpenGLData glPlane;
	// Generate G Buffer
	RenderTarget gBuffer;
	// Render Lights
	OpenGLData pointLight;

	


	unsigned int gBufferProgramID;
	unsigned int directionalLightProgramID;
	unsigned int pointLightProgramID;
	unsigned int spotLightProgramID;
	unsigned int compositeProgramID;

	// composite pass 
	
	
	virtual ~DeferredRendering();

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




	
	//void CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes);

private:

};

#endif // _DEFERREDRENDERING_H_