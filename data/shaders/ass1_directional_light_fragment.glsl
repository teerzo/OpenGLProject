#version 410

in vec2 vtx_uv;

out vec3 frg_color;

uniform vec3 light_dir;
uniform vec3 light_color;
uniform mat4 view;

uniform sampler2D position_texture;
uniform sampler2D normals_texture;

void main()
{ 
	vec4 light = vec4( light_dir.xyz, 1);
	vec4 direction = light; 
	vec3 normal_sample = texture(normals_texture, vtx_uv).xyz;
	vec3 position_sample = texture(position_texture, vtx_uv).xyz;

	vec3 N = normalize(normal_sample);

	float d = max(0, dot( N, -direction.xyz));

	//frg_color = vec3( 0.1, 0.1, 0.1 );
	frg_color = (light_color.xyz * d) + vec3( 0.1, 0.1, 0.1 );
	//frg_color.w = 1;
}