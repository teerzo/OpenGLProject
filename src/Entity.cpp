#include "Entity.h"

void Entity::generateGrid(unsigned int rows, unsigned int cols)
{
	// function to create a grid
	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = glm::vec4(
				(float)c, 0, (float)r, 1);
			// create some arbitrary colour based off something
			// that might not be related to tiling a texture
			glm::vec3 colour = glm::vec3(sinf((c / (float)(cols - 1)) *
				(r / (float)(rows - 1))));
			aoVertices[r * cols + c].colour = glm::vec4(colour, 1);
		}
	}
	// we’ll do more here soon!
	delete[] aoVertices;

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