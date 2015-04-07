#version 410

layout(location=0) in vec4 Position; 
layout(location=1) in vec2 UV;	

out vec2 tex_coord;

void main()
{
	tex_coord = UV;
	gl_Position = Position;
}