#version 410

out vec3 frg_color;

uniform vec3 light_view_position;
uniform vec3 light_color;
uniform float light_radius;

uniform sampler2D position_texture;
uniform sampler2D normals_texture;

void main()
{
	vec2 texCoord = gl_FragCoord.xy / textureSize(position_texture, 0).xy;
	vec3 normal = normalize(texture(normals_texture,texCoord).xyz);
	vec3 position = texture(position_texture, texCoord).xyz;

	vec3 toLight = light_view_position - position;

	float d  = max(0, dot(normal, normalize(toLight)));

	// simple linear falloff
	// Change this to quadratic falloff
	float falloff = 1 - min(1,length(toLight) / light_radius);

	frg_color = light_color * d * falloff;
}