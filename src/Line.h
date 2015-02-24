#ifndef _LINE_H_
#define _LINE_H_

#include "glm_header.h"

class Line
{
private:

public:
	glm::vec3 direction;
	float length;
	glm::vec3 start;
	glm::vec3 end;

	Line(glm::vec3 a_start, glm::vec3 a_direction)
	{
		start = a_start;
		direction = a_direction;
		end = a_start + direction;
	}
	void Update()
	{
		if (length < 10000 && end.y > 0  ) // far plane??
		{
			end += direction;
			length = glm::distance(start, end);
		} 
	}
	void Draw()
	{
		Gizmos::addLine(start, end, color.Green);
	}
};


#endif // _LINE_H_