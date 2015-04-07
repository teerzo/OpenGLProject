#version 410
layout(location = 0) in vec4 Position;
uniform mat4 light_matrix;
void main()
{
	gl_Position = light_matrix * Position;
}