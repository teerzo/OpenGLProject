#ifndef _RENDERING_GEOMETRY_H_
#define _RENDERING_GEOMETRY_H_

#include "Application.h"
#include "Vertex.h"


class Camera;
class FlyCamera;

class RenderingGeometry : public Application
{
private:
	glm::mat4 ProjectionMatrix;
	//Camera* sceneCamera;
	//FlyCamera* flyCamera;
	float m_fTimer;
	float m_fPreviousTime;
	
	// Vertex Array Object
	unsigned int m_VAO;
	// Vertex Buffer Object
	unsigned int m_VBO;
	// Index Buffer Object
	unsigned int m_IBO;
	// Combined Program Object, Contains m_VBO/m_IBO etc
	unsigned int m_ProgramID;

	// Shaders // 
	const char* vertexShader_Source;
	const char* fragmentShader_Source;

	unsigned int m_index_count;

public:
	virtual ~RenderingGeometry();

	virtual void setDefaults();
	virtual bool startup();
	virtual bool loadShaders();

	virtual void shutdown();

	virtual bool update();
	virtual void draw();


	void generateGrid(unsigned int rows, unsigned int cols);
	void generateShader();
};

#endif // _RENDERING_GEOMETRY_H_
