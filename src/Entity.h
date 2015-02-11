#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <glfw3.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"


#include "Object.h"

struct Vertex {
	glm::vec4 position;
	glm::vec4 colour;
};

class Entity : public Object
{
public:
	Entity();

	virtual void Update();
	virtual void Draw();
		



void generateGrid(unsigned int rows, unsigned int cols);


};


#endif // _ENTITY_H_