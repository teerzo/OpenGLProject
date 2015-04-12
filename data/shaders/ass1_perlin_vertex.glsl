#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec2 uv;

out vec3 vtx_position;

void main()
{
	vtx_position = position.xyz;
}