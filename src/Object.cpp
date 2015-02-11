#include "Object.h"

void Object::Update()
{
	
}

void Object::Draw()
{

}

glm::vec4 Object::GetGlobalPosition()
{
	m_WorldMatrix;
	return glm::vec4(0);
}
glm::vec4 Object::GetLocalPosition()
{
	m_LocalMatrix;
	return glm::vec4(0);
}