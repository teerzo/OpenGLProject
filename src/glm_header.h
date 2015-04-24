#ifndef _GLM_HEADER_H_
#define _GLM_HEADER_H_

#define GLM_SWIZZLE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct Color
{
	glm::vec4 Clear = glm::vec4(1, 1, 1, 0);
	glm::vec4 White = glm::vec4(1, 1, 1, 1);
	glm::vec4 Black = glm::vec4(0, 0, 0, 1);
	glm::vec4 Gray = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	glm::vec4 Red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 Blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 Green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 GreenClear = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 Yellow = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	glm::vec4 Orange = glm::vec4(1.0f, 0.4f, 0.0f, 1.0f);

};

static Color color;

#endif