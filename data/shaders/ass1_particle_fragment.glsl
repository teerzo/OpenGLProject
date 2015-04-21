#version 410

in vec4 vtx_position;
in vec4 vtx_color;
in vec4 vtx_normal;

layout(location = 0) out vec3 albedo;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;

void main()
{
	albedo = vtx_color.xyz;
	position = vtx_position.xyz;
	normal = vtx_normal.xyz;
}