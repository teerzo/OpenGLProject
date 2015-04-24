#ifndef _BOUNDING_SHAPES_H_
#define _BOUNDING_SHAPES_H_

#include "glm_header.h"
#include <vector>

class AABB
{
private:

public:
	AABB() { reset(); }
	~AABB() {}
	void reset()
	{
		min.x = min.y = min.z = 1e37f;
		max.x = max.y = max.z = -1e37f;
	}
	void fit(const std::vector<glm::vec3>& points)
	{
		for (auto& p : points)
		{
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;

			if (p.x < max.x) max.x = p.x;
			if (p.y < max.y) max.y = p.y;
			if (p.z < max.z) max.z = p.z;
		}
	}

	glm::vec3 min;
	glm::vec3 max;
};

class BoundingSphere
{
private:

public:
	BoundingSphere() : centre(0), radius(0) {}
	~BoundingSphere() {}
	void fit(const std::vector<glm::vec3>& points) 
	{
	glm::vec3 min(1e37f), max(-1e37f);

		for (auto& p : points) {
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;
			if (p.x > max.x) max.x = p.x;
			if (p.y > max.y) max.y = p.y;
			if (p.z > max.z) max.z = p.z;
		}
		centre = (min + max) * 0.5f;
		radius = glm::distance(min, centre);
	}

	glm::vec3 centre;
	float radius;
};

#endif // _BOUNDING_SHAPES_H_