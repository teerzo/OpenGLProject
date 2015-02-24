#include "Entity.h"


#include "Vertex.h"
Entity::Entity()
{
	GenerateCube();
}

void Entity::Update()
{
	Object::Update();
	
}

void Entity::Draw()
{
	Object::Draw();
	glBindVertexArray(m_cube.m_VAO);
	glDrawElements(GL_TRIANGLES, m_cube.m_index_count, GL_UNSIGNED_INT, 0);
}

void Entity::GenerateCube()
{
	VertexNormal vertex_data[8];

	vertex_data[0].Position = glm::vec4(-1, 1, -1, 1);
	vertex_data[1].Position = glm::vec4(-1, 1, 1, 1);
	vertex_data[2].Position = glm::vec4(1, 1, 1, 1);
	vertex_data[3].Position = glm::vec4(1, 1, -1, 1);

	vertex_data[4].Position = glm::vec4(-1, -1, -1, 1);
	vertex_data[5].Position = glm::vec4(-1, -1, 1, 1);
	vertex_data[6].Position = glm::vec4(1, -1, 1, 1);
	vertex_data[7].Position = glm::vec4(1, -1, -1, 1);

	vertex_data[0].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[1].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[2].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[3].Normal = glm::vec4(0, 1, 0, 0);

	vertex_data[4].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[5].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[6].Normal = glm::vec4(0, 1, 0, 0);
	vertex_data[7].Normal = glm::vec4(0, 1, 0, 0);

	vertex_data[0].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[1].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[2].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[3].Tangent = glm::vec4(1, 0, 0, 0);

	vertex_data[4].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[5].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[6].Tangent = glm::vec4(1, 0, 0, 0);
	vertex_data[7].Tangent = glm::vec4(1, 0, 0, 0);

	vertex_data[0].UV = glm::vec2(0, 0);
	vertex_data[1].UV = glm::vec2(0, 1);
	vertex_data[2].UV = glm::vec2(1, 1);
	vertex_data[3].UV = glm::vec2(1, 0);

	vertex_data[4].UV = glm::vec2(0, 0);
	vertex_data[5].UV = glm::vec2(0, 1);
	vertex_data[6].UV = glm::vec2(1, 1);
	vertex_data[7].UV = glm::vec2(1, 0);

	unsigned int index_data[6*6] = { 0, 4, 7, 0, 7, 3, // front 
									 3, 7, 6, 3, 6, 2, // right side
									 2, 6, 5, 2, 5, 1, // back 
									 1, 5, 4, 1, 4, 0, // left side
									 4, 5, 6, 4, 6, 7, // top 
									 2, 3, 0, 2, 0, 1 }; // bottom
	m_cube.m_index_count = 6*6;

	glGenVertexArrays(1, &m_cube.m_VAO);

	glGenBuffers(1, &m_cube.m_VBO);
	glGenBuffers(1, &m_cube.m_IBO);
	glBindVertexArray(m_cube.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_cube.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal) * 8, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (6*6), index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // Tangent
	glEnableVertexAttribArray(3); // UV

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)(sizeof(glm::vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}