#version 410

in vec4 Position;
in vec2 UV;

layout(location=0) in vec4 position; 
layout(location=1) in vec2 uv;	

out vec4 reflected_screen_pos;

uniform mat4 projection_view;
uniform mat4 reflected_projection_view;

void main()
{
	reflected_screen_pos = reflected_projection_view * position;
	gl_Position = projection_view * position;
}