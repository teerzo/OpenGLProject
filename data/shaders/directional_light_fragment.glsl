#version 410

in vec2 vTexCoord;

out vec3 out_color;

uniform vec3 light_dir;
uniform vec3 light_color;

uniform sampler2D position_texture;
uniform sampler2D normals_texture;

void main()
{
	vec3 normal_sample = texture(normals_texture, vTexCoord).xyz;
	vec3 position_sample = texture(position_texture, vTexCoord).xyz;

	vec3 N = normalize(normal_sample);

	float d = max(0,dot(-light_dir, N));

	out_color = (light_color * d) + vec3( 0.2, 0.2, 0.2 );
}