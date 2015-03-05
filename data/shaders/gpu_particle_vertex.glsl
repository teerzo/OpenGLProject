#version 410

layout(location=0) in vec3 position;
layout(location=1) in vec3 velocity;
layout(location=2) in float lifetime;
layout(location=3) in float lifespan;

out vec3 geometry_position;
out float geometry_lifetime;
out float geometry_lifespan;

void main()
{
	geometry_position = position;
	geometry_lifetime = lifetime;
	geometry_lifespan = lifespan;
}