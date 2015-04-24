#include "Object.h"
#include "Gizmos.h"

void Object::Update()
{
	// Base Update
	if (is_active)
	{
		// Update
	}
}
void Object::Draw()
{
	// Base Draw
	if (is_visible)
	{
		// Draw
		Gizmos::addTransform(matrix_world, 1.0f);
	}
}
void Object::SetWorldPosition(const glm::vec3 position)
{
	matrix_world[3].xyz = position;
}
void Object::SetLocalPosition(const glm::vec3 position)
{
	matrix_local[3].xyz = position;
}
glm::mat4 Object::GetWorld()	{ return matrix_world; }
glm::mat4 Object::GetLocal()	{ return matrix_local;  }
glm::vec3 Object::GetRight()	{ return matrix_world[0].xyz; }
glm::vec3 Object::GetUp()		{ return matrix_world[1].xyz; }
glm::vec3 Object::GetForward()	{ return matrix_world[2].xyz; }

