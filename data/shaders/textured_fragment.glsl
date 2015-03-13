#version 410

in vec2 out_uv;

out vec4 frag_color;
uniform sampler2D diffuse;

uniform vec3 cameraPos;

void main()
{
	frag_color = texture(diffuse, out_uv);
};
	