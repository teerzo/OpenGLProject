#version 410

in vec2 vtx_uv;

out vec4 FragColour;

uniform sampler2D albedo_texture;
uniform sampler2D light_texture;

void main()
{
	vec3 light = texture(light_texture, vtx_uv).rgb;
	vec3 albedo = texture(albedo_texture, vtx_uv).rgb;

	FragColour = vec4( albedo, 1 );
	//FragColour = vec4( light, 1 );
	//FragColour = vec4( albedo * light, 1 );	
}