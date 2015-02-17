#ifndef _INTROTOOPENGL_H_
#define _INTROTOOPENGL_H_

#include "Application.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class IntroToOpenGL : public Application 
{
private:


public:
	virtual ~IntroToOpenGL();
	// Base Functions 
	virtual void setDefaults();
	virtual bool startup();
	virtual void shutdown();
	virtual bool update();
	virtual void draw();
	//virtual void checkKeys();

	// Project Specific 
	
	float rotate_x1;
	float rotate_x2;
	float rotate_z1;
	float rotate_z2;
	

};


#endif // _INTROTOOPENGL_H_