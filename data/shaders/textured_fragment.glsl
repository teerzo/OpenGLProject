#version 410
in vec4 out_color;
in vec2 out_uv;

out vec4 frag_color;
uniform sampler2D diffuse;
uniform float timer;
uniform vec3 cameraPos;

void main()
{
	vec2 UV = out_uv;
	vec4 color = out_color;
	UV.x += sin( timer );
	UV.y += cos( timer );
	//color.x += cos(-timer);
	//color.y += cos(-timer);
	//color.z += cos(-timer);

	//UV.x = (cameraPos.x / 1) + 1;
	//UV.y = (cameraPos.z / 1) + 1;
	frag_color = color * texture(diffuse, UV);
	//frag_color = texture(diffuse, UV);
};
	