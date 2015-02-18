#include "Entity.h"

void Entity::generateGrid(unsigned int rows, unsigned int cols)
{
	
}

void Entity::Update()
{
	Object::Draw();
	glm::vec4 pos = GetLocalPosition();
}

void Entity::Draw()
{
	Object::Draw();
}