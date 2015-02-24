#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "glm_header.h"

struct Vertex
{
	glm::vec4 Position;
	glm::vec4 Color;
	glm::vec2 UV;
	//glm::vec4 Normal;
};

struct VertexNormal
{
	glm::vec4 Position;
	glm::vec4 Normal;
	glm::vec4 Tangent;
	glm::vec2 UV;

};

struct OpenGLData
{
	// Vertex Array Object
	unsigned int m_VAO;
	// Vertex Buffer Object
	unsigned int m_VBO;
	// Index Buffer Object
	unsigned int m_IBO;
	unsigned int m_index_count;
};


#endif _VERTEX_H_