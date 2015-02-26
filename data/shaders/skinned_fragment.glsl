#version 410

in vec2 out_uv;

out vec4 frag_color;

uniform sampler2D Diffuse_Tex;

void main()
{
	vec2 UV = out_uv;
	frag_color = texture(Diffuse_Tex, UV);
};
	