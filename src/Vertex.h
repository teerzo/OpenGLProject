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

struct VertexParticle
{
	glm::vec4 Position;
	glm::vec4 Color;

};

struct VertexUV
{
	glm::vec4 Position;
	glm::vec2 UV;
};

struct Rectangle
{
	float topLeft;
	float bottomLeft;
	float topRight;
	float bottomRight;
	Rectangle()
	{
		topLeft = 1.0f;
		bottomLeft = 1.0f;
		topRight = 1.0f;
		bottomRight = 1.0f;
	}
	Rectangle(float square)
	{
		topLeft = square;
		bottomLeft = square;
		topRight = square;
		bottomRight = square;
	}
	Rectangle(float a, float b, float c, float d)
	{
		topLeft = a;
		bottomLeft = b;
		topRight = c;
		bottomRight = d;
	}	
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
	//std::vector<glm::vec3> m_positions;
	//std::vector<glm::vec3> m_normals;

};


#endif _VERTEX_H_