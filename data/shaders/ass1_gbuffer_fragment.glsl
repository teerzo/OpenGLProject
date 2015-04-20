#version 410

in vec4 vtx_position;
in vec4 vtx_normal;
in vec4 vtx_offset;
in vec2 vtx_uv;

layout(location = 0) out vec3 albedo;
layout(location = 1) out vec3 position;
layout(location = 2) out vec3 normal;

uniform mat4 projection_view;
uniform float timer;

uniform sampler2D lava_texture;
uniform sampler2D soul_spear_texture;
uniform sampler2D mesh_texture;

void main()
{
	// we simply output the data
	// Note: you could use a material colour,
	// or sample a texture for albedo

	//albedo = vtx_normal.xyz;
	//albedo = vec3(1,1,0);
	
	float speed = 10;

	albedo = vec3(texture( mesh_texture, vtx_uv).xyz);

	//albedo = vec3(texture( lava_texture, (vtx_uv * 5) + (speed * (timer/1000)) ).xyz);
	
	position = vtx_position.xyz;
	normal = vtx_normal.xyz;
}