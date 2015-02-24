#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "gl_core_4_4.h"
#include <glfw3.h>
#include "Vertex.h"


class  Object
{
protected:

	glm::mat4 m_WorldMatrix;
	glm::mat4 m_LocalMatrix;
	bool m_bIsVisible;
		
public:
	virtual void Update();
	virtual void Draw();

	// Get Globals
	glm::vec3 GetGlobalPosition();

	// Set Globals
	void SetGlobalPosition(glm::vec3 a_position);

	// Get Locals
	glm::vec3 GetLocalPosition();

	// Set Locals
	void SetLocalPosition(glm::vec3 a_position);
};



#endif // _OBJECT_H_