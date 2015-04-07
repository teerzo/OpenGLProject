#version 410

in vec4 vtx_position;
in vec4 vtx_normal;
in vec4 vtx_offset;

layout(location = 0) out vec3 albedo;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;

uniform mat4 projection_view;

void main()
{
	// we simply output the data
	// Note: you could use a material colour,
	// or sample a texture for albedo
	//albedo = vec3(1,1,1);

	albedo = vtx_normal.xyz;
	position = vtx_position.xyz;
	normal = vtx_normal.xyz;
}