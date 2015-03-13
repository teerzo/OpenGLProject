#version 410
layout(location=0) in vec4 Position; 
layout(location=1) in vec2 UV;	
out vec2 out_uv;

uniform mat4 ProjectionView;

void main()
{
	out_uv = UV;
	gl_Position = ProjectionView * Position;
}