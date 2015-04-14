#version 410

in vec2 vtx_uv;

out vec3 frg_color;

uniform vec3 light_dir;
uniform vec3 light_color;

uniform sampler2D position_texture;
uniform sampler2D normals_texture;

void main()
{
	vec3 normal_sample = texture(normals_texture, vtx_uv).xyz;
	vec3 position_sample = texture(position_texture, vtx_uv).xyz;

	vec3 N = normalize(normal_sample);

	float d = max(0, dot( N, -light_dir));

	frg_color = (light_color.xyz * d) + vec3( 0.01, 0.01, 0.01 );
	//frg_color.w = 1;
}