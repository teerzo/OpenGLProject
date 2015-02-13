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



#endif _VERTEX_H_