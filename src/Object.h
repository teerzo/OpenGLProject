#ifndef _OBJECT_H_
#define _OBJECT_H_


#include <glfw3.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

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
	glm::vec4 GetGlobalPosition();

	// Get Locals
	glm::vec4 GetLocalPosition();

};



#endif // _OBJECT_H_