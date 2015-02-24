#include "Object.h"

void Object::Update()
{
	// Base Update

}

void Object::Draw()
{
	// Base Draw

}

// Get Globals
glm::vec3 Object::GetGlobalPosition()
{
	return m_WorldMatrix[3].xyz;
}

// Set Globals
void Object::SetGlobalPosition(glm::vec3 a_position)
{
	m_WorldMatrix[3].xyz = a_position;
}

// Get Locals
glm::vec3 Object::GetLocalPosition()
{
	return m_LocalMatrix[3].xyz;
}

// Set Locals
void Object::SetLocalPosition(glm::vec3 a_position)
{
	m_LocalMatrix[3].xyz = a_position;
}