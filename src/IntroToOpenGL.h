#ifndef _INTROTOOPENGL_H_
#define _INTROTOOPENGL_H_

#include "Application.h"
#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class IntroToOpenGL : public Application 
{
private:
	float timer;
	glm::mat4 ProjectionMatrix;

public:
	virtual ~IntroToOpenGL();

	virtual void setDefaults();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();
	
	float rotate_x1;
	float rotate_x2;
	float rotate_z1;
	float rotate_z2;
	
	glm::vec4 White;
	glm::vec4 Red;
	glm::vec4 Yellow;
	glm::vec4 Gray;
	glm::vec4 Black;


};


#endif // _INTROTOOPENGL_H_