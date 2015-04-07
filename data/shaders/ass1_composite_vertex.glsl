#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec2 vtx_uv;

uniform mat4 projection_view;

void main() 
{
	vtx_uv = uv;
	gl_Position = position;
}