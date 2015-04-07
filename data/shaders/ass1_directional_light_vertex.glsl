#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

uniform mat4 projection_view;

out vec2 frg_uv;

void main() 
{
	frg_uv = uv;
	gl_Position = position;
}