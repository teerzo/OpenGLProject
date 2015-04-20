#version 410
layout(location=0) in vec4 Position; 
layout(location=1) in vec4 Color;
	
out vec4 out_color;
uniform mat4 ProjectionView;
void main()
{
	out_color = Color;
	gl_Position = ProjectionView * Position;
}