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
	UV.x += cameraPos.x * 0.1 ;
	UV.y += cameraPos.z * 0.1 ;
	color.x = cameraPos.x / 1;
	color.y = cameraPos.y / 1;
	color.z = cameraPos.z / 1;

	//UV.x = (cameraPos.x / 1) + 1;
	//UV.y = (cameraPos.z / 1) + 1;
	frag_color = color * texture(diffuse, UV);
	//frag_color = texture(diffuse, UV);
};
	