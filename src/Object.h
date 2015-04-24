#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "gl_core_4_4.h"
#include <glfw3.h>
#include "Vertex.h"


class  Object
{
public:
	Object* parent;
	glm::mat4 matrix_world;
	glm::mat4 matrix_local;
	bool is_visible;
	bool is_active;
		
	void Update();
	void Draw();
	// Sets
	void SetWorldPosition(const glm::vec3 position);
	void SetLocalPosition(const glm::vec3 position);
	// Gets
	glm::mat4 GetWorld();
	glm::mat4 GetLocal();
	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
};
#endif // _OBJECT_H_